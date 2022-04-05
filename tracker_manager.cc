#include "tracker_manager.h"

#include <iostream>
#include <utility>
#include <vector>

#include "seeso/util/display.h"

namespace sample {

static std::vector<float> getWindowRectWithPadding(const char* window_name, int padding = 30) {
  const auto window_rect = seeso::getWindowRect(window_name);
  return {
    static_cast<float>(window_rect.x + padding),
    static_cast<float>(window_rect.y + padding),
    static_cast<float>(window_rect.x + window_rect.width - padding),
    static_cast<float>(window_rect.y + window_rect.height - padding)};
}

void TrackerManager::OnGaze(uint64_t timestamp,
                            float x, float y,
                            SeeSoTrackingState tracking_state,
                            SeeSoEyeMovementState eye_movement_state) {
  if (tracking_state != kSeeSoTrackingSuccess) {
    on_gaze_(0, 0, false);
    return;
  }

  // Convert the gaze point(in display pixels) to the pixels of the OpenCV window
  // (because top left of the window may not be [0, 0])
  // This can be omitted if the top left of the window is always [0, 0], such as fullscreen-guaranteed applications
  auto winPos = seeso::getWindowPosition(window_name_);
  x -= static_cast<float>(winPos.x);
  y -= static_cast<float>(winPos.y);

  on_gaze_(static_cast<int>(x), static_cast<int>(y), true);
}

void TrackerManager::OnAttention(uint64_t timestampBegin, uint64_t timestampEnd, float score) {
  std::cout << "Attention: " << timestampBegin << " " << timestampEnd << " " << score << '\n';
}

void TrackerManager::OnBlink(uint64_t timestamp, bool isBlinkLeft, bool isBlinkRight, bool isBlink, float eyeOpenness) {
//  std::cout << "Blink: " << isBlink << ", " << isBlinkLeft << ", " << isBlinkRight << '\n'
//            << "EyeOpenness: " << eyeOpenness << '\n';
}

void TrackerManager::OnDrowsiness(uint64_t timestamp, bool isDrowsiness) {
//  std::cout << "Drowsiness: " << isDrowsiness << '\n';
}

void TrackerManager::OnCalibrationProgress(float progress) {
  on_calib_progress_(progress);
  gaze_tracker_.startCollectSamples();
}

void TrackerManager::OnCalibrationNextPoint(float next_point_x, float next_point_y) {
  const auto winPos = seeso::getWindowPosition(window_name_);
  const auto x = static_cast<int>(next_point_x - static_cast<float>(winPos.x));
  const auto y = static_cast<int>(next_point_y - static_cast<float>(winPos.y));

  on_calib_next_point_(x, y);
}

void TrackerManager::OnCalibrationFinish(const std::vector<float> &calib_data) {
  on_calib_finish_(calib_data);
  calibrating_.store(false, std::memory_order_release);
}

bool TrackerManager::initialize(const std::string &license_key, const SeeSoStatusModuleOptions& status_option) {
  const auto code = gaze_tracker_.initialize(license_key, status_option);
  if (code != 0) {
    std::cerr << "Failed to authenticate (code: " << code << " )\n";
    return false;
  }

  // Set approximate distance between the face and the camera.
  gaze_tracker_.setFaceDistance(60);

  // Set additional options
  gaze_tracker_.setAttentionInterval(10);

  // Attach callbacks to seeso::GazeTracker
  gaze_tracker_.setGazeCallback(this);
  gaze_tracker_.setCalibrationCallback(this);
  gaze_tracker_.setUserStatusCallback(this);

  return true;
}

void TrackerManager::setDefaultCameraToDisplayConverter(const seeso::DisplayInfo &display_info) {
  gaze_tracker_.converter() = seeso::makeDefaultCameraToDisplayConverter<float>(
    static_cast<float>(display_info.widthPx), static_cast<float>(display_info.heightPx),
    display_info.widthMm, display_info.heightMm);
}

bool TrackerManager::addFrame(std::int64_t timestamp, const cv::Mat &frame) {
  return gaze_tracker_.addFrame(timestamp, frame.data, frame.cols, frame.rows);
}

void TrackerManager::startFullWindowCalibration(SeeSoCalibrationPointNum target_num, SeeSoCalibrationAccuracy accuracy) {
  bool expected = false;
  if (!calibrating_.compare_exchange_strong(expected, true))
    return;

  on_calib_start_();

  // Delay start to show description message
  delayed_calibration_ = std::async(std::launch::async, [=]() {
    // TODO(Tony): Handle force exit
    std::this_thread::sleep_for(std::chrono::seconds(3));
    const auto window_rect = getWindowRectWithPadding(window_name_.c_str());
    gaze_tracker_.startCalibration(target_num, accuracy,
                                   window_rect[0], window_rect[1], window_rect[2], window_rect[3]);
  });
}

void TrackerManager::setWholeScreenToAttentionRegion(const seeso::DisplayInfo& display_info) {
  gaze_tracker_.setAttentionRegion(0, 0,
                             static_cast<float>(display_info.widthPx), static_cast<float>(display_info.heightPx));
}

} // namespace sample
