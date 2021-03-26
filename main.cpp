#include <iostream>
#include <type_traits>
#include <thread>

#include <windows.h>

#include <opencv2/opencv.hpp>

#include "seeso/eye_tracker.h"
#include "seeso/util/display.h"
#include "seeso/util/coord_converter.h"

#include "callback.h"


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
  auto displays = seeso::getDisplayLists();
  if(displays.empty()) {
    std::cerr << "Cannot find display\n";
    return EXIT_FAILURE;
  }

  printDisplays(displays);
  auto& main_display = displays[0];

  std::unique_ptr<std::remove_pointer_t<HINSTANCE>, decltype(&FreeLibrary)>
      hGetProcIDDLL = {LoadLibrary("seeso.dll"), FreeLibrary};
  if (!hGetProcIDDLL) {
    std::cerr << "could not load the dynamic library seeso.dll"
                 " (Error code: " << GetLastError() << '\n';
    return EXIT_FAILURE;
  }

  // load library
  auto eye_tracker = std::make_unique<seeso::EyeTracker>(hGetProcIDDLL.get());
  std::cout << "SeeSo Version: " << eye_tracker->getVersion() << std::endl;

  // authenticate
  const char* license_key = "dev_ddfwj2tj1i9tjd7850daon39plhqy5kwdjcx7xoq";
  auto code = eye_tracker->initialize(license_key);
  if(code != 0) {
    std::cerr << "Failed to authenticate "
              << "(code: " << code << ")\n";
    return EXIT_FAILURE;
  }

  // set callback
  auto callback = Callback(main_display);
  eye_tracker->setCallbackInterface(&callback);

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
  const char* window_name = "camera";
  cv::namedWindow(window_name);

  for(;;) {
    video >> frame;
    if(frame.empty()) break;

    cv::cvtColor(frame, input, cv::COLOR_BGR2RGB);

    eye_tracker->addFrame(
        std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count(),
        input.data, input.cols, input.rows);

    cv::imshow(window_name, frame);
    if(cv::waitKey(1) == 27 /* ESC */ ) break;
  }
  cv::destroyWindow(window_name);

  // free dll after eye_tracker is destroyed
  eye_tracker.reset();
  hGetProcIDDLL.reset();

  return EXIT_SUCCESS;
}
