#ifndef SEESO_SAMPLE_TRACKER_MANAGER_H_
#define SEESO_SAMPLE_TRACKER_MANAGER_H_

#include <atomic>
#include <future>
#include <memory>
#include <string>
#include <vector>

#include "seeso/gaze_tracker.h"
#include "seeso/util/display.h"

#include "opencv2/opencv.hpp"

#include "simple_signal.h"

namespace sample {

class TrackerManager :
  public seeso::IGazeCallback,
  public seeso::ICalibrationCallback,
  public seeso::IUserStatusCallback {
 public:
  TrackerManager() = default;

  bool initialize(const std::string &license_key, const SeeSoStatusModuleOptions& status_option);

  void setDefaultCameraToDisplayConverter(const seeso::DisplayInfo& display_info);

  bool addFrame(std::int64_t timestamp, const cv::Mat& frame);

  void startFullWindowCalibration(SeeSoCalibrationPointNum target_num, SeeSoCalibrationAccuracy accuracy);

  void setWholeScreenToAttentionRegion(const seeso::DisplayInfo& display_info);

  // message senders
  signal<void(int, int, bool)> on_gaze_;
  signal<void(float)> on_calib_progress_;
  signal<void(int, int)> on_calib_next_point_;
  signal<void()> on_calib_start_;
  signal<void(const std::vector<float>&)> on_calib_finish_;

  std::string window_name_;

 private:
  void OnGaze(uint64_t timestamp, float x, float y,
              SeeSoTrackingState tracking_state, SeeSoEyeMovementState eye_movement_state) override;
  void OnAttention(uint64_t timestampBegin, uint64_t timestampEnd, float score) override;
  void OnBlink(uint64_t timestamp, bool isBlinkLeft, bool isBlinkRight, bool isBlink, float eyeOpenness) override;
  void OnDrowsiness(uint64_t timestamp, bool isDrowsiness) override;

  void OnCalibrationProgress(float progress) override;
  void OnCalibrationNextPoint(float next_point_x, float next_point_y) override;
  void OnCalibrationFinish(const std::vector<float> &calib_data) override;

  seeso::GazeTracker gaze_tracker_;
  std::future<void> delayed_calibration_;
  std::atomic_bool calibrating_{false};
};

} // namespace sample

#endif // SEESO_SAMPLE_TRACKER_MANAGER_H_
