#include <iostream>
#include <type_traits>
#include <thread>

#include <windows.h>

#include <opencv2/opencv.hpp>
#include <memory>

#include "seeso/eye_tracker.h"
#include "seeso/framework/user_status_option.h"
#include "seeso/util/display.h"
#include "seeso/util/coord_converter.h"

#include "callback.h"
#include "view.h"

void printDisplays(const std::vector<seeso::DisplayInfo>& displays) {
  for(const auto& display : displays) {
    std::cout << "\nDisplay Name    : " << display.displayName
              << "\nDisplay String  : " << display.displayString
              << "\nDisplayFlags    : " << display.displayStateFlag
              << "\nDisplayId       : " << display.displayId
              << "\nDisplayKey      : " << display.displayKey
              << "\nSize(mm)        : " << display.widthMm << "x" << display.heightMm
              << "\nSize(px)        : " << display.widthPx << "x" << display.heightPx
              << "\n";
  }
}

int main() {
  const auto displays = seeso::getDisplayLists();
  if(displays.empty()) {
    std::cerr << "Cannot find display\n";
    return EXIT_FAILURE;
  }

  printDisplays(displays);
  const auto& main_display = displays[0];

  // initialize seeso library
  seeso::global_init();

  // load library
  auto eye_tracker = std::make_unique<seeso::EyeTracker>();
  std::cout << "SeeSo Version: " << seeso::getVersionStr() << std::endl;

  // authenticate
  const char* license_key = "PUT YOUR LICENSE KEY HERE";

  auto userStatusOption = seeso::UserStatusOption();;
  userStatusOption.setUseAttention(true);
  userStatusOption.setUseBlink(true);
  userStatusOption.setUseDrowsiness(true);

  auto code = eye_tracker->initialize(license_key, userStatusOption);
  if(code != 0) {
    std::cerr << "Failed to authenticate "
              << "(code: " << code << ")\n";
    return EXIT_FAILURE;
  }

  // Initialization check
  if (eye_tracker->isTrackerInitialized()) {
    std::cout << "Tracker Initialized" << std::endl;
  }

  // set face distance
  eye_tracker->setFaceDistance(60);

  // set callback
  Callback callback(main_display);

  // use callback in eyetracker.
  eye_tracker->setGazeCallback(&callback);
  eye_tracker->setCalibrationCallback(&callback);
  eye_tracker->setUserStatusCallback(&callback);

  // set UserStaus Attention Interval
  eye_tracker->setAttentionInterval(30);

  // opencv camera example
  int camera_index = 0;
  cv::VideoCapture video(camera_index);

  if(!video.isOpened()) {
    std::cerr << "Failed to open camera\n";
    return EXIT_FAILURE;
  }

  video.set(cv::CAP_PROP_FRAME_WIDTH, 640);
  video.set(cv::CAP_PROP_FRAME_HEIGHT, 480);
  cv::Mat frame, input;
  const char* window_name = "seesosample";
  auto view = std::make_shared<seeso::View>(1280, 960, window_name);

  // use view instance when the callback is called.
  callback.registerView(view);

  // set status region Bound
  seeso::CoordConverter pc = seeso::CoordConverter(main_display);
  auto lt = pc.screenToCamera(seeso::Point<float>(0, 0));
  auto rb = pc.screenToCamera(seeso::Point<float>(main_display.widthPx, main_display.heightPx));
  eye_tracker->setTargetBoundRegion(lt.x, lt.y, rb.x, rb.y);

  for(;;) {
    video >> frame;
    if(frame.empty()) break;

    cv::cvtColor(frame, input, cv::COLOR_BGR2RGB);

    eye_tracker->addFrame(
        std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count(),
        input.data, input.cols, input.rows);

    // pass video frame. frame is an element for drawing the view.
    view->setFrame(frame);

    // Since we have all the elements, draw 'seesosample' window.
    int key = view->draw();
    if (key == 27) {
      break;
    }
  }
  view->closeWindow();


  return EXIT_SUCCESS;
}
