#include <iostream>
#include <thread>
#include <stdexcept>
#include <memory>

#include <opencv2/opencv.hpp>

#include "seeso/gaze_tracker.h"
#include "seeso/util/display.h"

#include "tracker_manager.h"
#include "view.h"
#include "camera_thread.h"

#ifdef SEESO_TEST_KEY
#  define SEESO_STRINGFY_IMPL(x) #x
#  define SEESO_STRINGFY(x) SEESO_STRINGFY_IMPL(x)
const char* license_key = SEESO_STRINGFY(SEESO_TEST_KEY);
#else
const char* license_key = "YOUR LICENSE KEY HERE";
#endif

// This project is a GUI(using OpenCV) example using SeeSo SDK
// See https://docs.seeso.io/docs for more information

void printDisplays(const std::vector<seeso::DisplayInfo>& displays);

int main() {
  // Initialize a SeeSo library
  // This must be called before calling any other seeso functions
  try {
    seeso::global_init();
  } catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
    return EXIT_FAILURE;
  }

  // Get display information
  const auto displays = seeso::getDisplayLists();
  if (displays.empty()) {
    std::cerr << "Cannot find displays\n";
    return EXIT_FAILURE;
  }
  printDisplays(displays);

  // Create gaze-tracker manager
  auto tracker_manager = std::make_shared<sample::TrackerManager>();
  auto tracker_manager_ptr = tracker_manager.get();

  // Set additional feature(user status) options
  auto userStatusOption = seeso::UserStatusOption().useAttention()
                                                   .useBlink()
                                                   .useDrowsiness();

  // Authenticate and initialize GazeTracker
  auto code = tracker_manager->initialize(license_key, userStatusOption);
  if (!code)
    return EXIT_FAILURE;


  // Change default coordinate system from camera-millimeters to display-pixels
  // This assumes that the camera is located at the top-center of the main display
  const auto& main_display = displays[0];
  tracker_manager->setDefaultCameraToDisplayConverter(main_display);


  // Create and run OpenCV camera in concurrent thread
  int camera_index = 0;
  sample::CameraThread camera_thread;
  if (!camera_thread.run(camera_index))
    return EXIT_FAILURE;

  // Create a view for drawing GUI
  const char* window_name = "seeso-sample";
  auto view = std::make_shared<sample::View>(main_display.widthPx * 2 / 3, main_display.heightPx * 2 / 3, window_name);
  auto view_ptr = view.get();
  tracker_manager->window_name_ = window_name;


  /// Adding listeners to events

  // Show gaze point according to the value. Red means the SeeSo cannot inference the gaze point
  tracker_manager->on_gaze_.connect([=](int x, int y, bool valid) {
    if (valid) {
      view_ptr->gaze_point_.center = {x, y};
      view_ptr->gaze_point_.color = {0, 220, 220};
    } else {
      view_ptr->gaze_point_.color = {0, 0, 220};
    }
    view_ptr->gaze_point_.visible = true;
  }, view);

  // Change UI elements state while calibrating
  tracker_manager->on_calib_start_.connect([=]() {
    view_ptr->calibration_desc_.visible = true;
    for (auto& desc : view_ptr->desc_)
      desc.visible = false;
    view_ptr->frame_.visible = false;
  }, view);
  tracker_manager->on_calib_finish_.connect([=](const std::vector<float>& data) {
    view_ptr->calibration_desc_.visible = false;
    view_ptr->calibration_point_.visible = false;
    for (auto& desc : view_ptr->desc_)
      desc.visible = true;
    view_ptr->frame_.visible = true;
  }, view);
  tracker_manager->on_calib_next_point_.connect([=](int x, int y) {
    view_ptr->calibration_point_.center = {x, y};
    view_ptr->calibration_point_.visible = true;
    view_ptr->calibration_desc_.visible = false;
  }, view);
  tracker_manager->on_calib_progress_.connect([=](float progress) {
    std::cout << '\r' << progress * 100 << '%';
  }, view);


  /// Add camera frame listener
  // 1. draw the preview to the view
  camera_thread.on_frame_.connect([=](const cv::Mat& frame) {
    view_ptr->frame_.buffer = frame;
  }, view);

  // 2. Pass the frame and the current timestamp in milliseconds to the SeeSo SDK
  camera_thread.on_frame_.connect([=](const cv::Mat& frame) {
    static auto cvt = new cv::Mat();
    static const auto current_time = [] {
      using clock = std::chrono::steady_clock;
      return std::chrono::duration_cast<std::chrono::milliseconds>(clock::now().time_since_epoch()).count();
    };
    cv::cvtColor(frame, *cvt, cv::COLOR_BGR2RGB);
    tracker_manager_ptr->addFrame(current_time(), *cvt);
  }, tracker_manager);


  while (true) {
    // Draw a window.
    int key = view->draw();

    if (key == 27/* ESC */) {
      break;
    } else if (key == 'c' || key == 'C') {
      tracker_manager->startFullWindowCalibration(
          seeso::TargetNum::FIVE, seeso::CalibrationAccuracy::DEFAULT);
    }
  }
  view->closeWindow();


  return EXIT_SUCCESS;
}

void printDisplays(const std::vector<seeso::DisplayInfo>& displays) {
  for (const auto& display : displays) {
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
