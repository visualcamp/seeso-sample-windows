/**
 * Written by Dean on 2021-04-07.
 * When EyeTracker class is initialized normally, the overloaded functions of the 'Callback' class are called in time.
 * */

#ifndef SEESO_WINDOWS_CALLBACK_H_
#define SEESO_WINDOWS_CALLBACK_H_

#include "seeso/callback/igaze_callback.h"
#include "seeso/callback/icalibration_callback.h"
#include "seeso/callback/iuser_status_callback.h"
#include "seeso/util/coord_converter.h"

#include "view.h"

#include <memory>
#include <vector>

class Callback :
    public seeso::IGazeCallback,
    public seeso::ICalibrationCallback,
    public seeso::IUserStatusCallback
{
 public:
  explicit Callback(const seeso::DisplayInfo &display);

  void OnGaze(uint64_t timestamp, float x, float y,
              seeso::TrackingState tracking_state, seeso::EyeMovementState eye_movement_state) override;
    
  void OnAttention(uint64_t timestamp, float score) override;
  void OnBlink(uint64_t timestamp, bool isBlinkLeft, bool isBlinkRight, bool isBlink, float eyeOpenness) override;
  void OnDrowsiness(uint64_t timestamp, bool isDrowsiness) override;

  void OnCalibrationProgress(float progress) override;
  void OnCalibrationNextPoint(float next_point_x, float next_point_y) override;
  void OnCalibrationFinish(const std::vector<float>& calib_data) override;

  void registerView(std::shared_ptr<seeso::View> view);

 private:
  std::shared_ptr<seeso::View> view;
  seeso::CoordConverter pc;
};


#endif //SEESO_WINDOWS_CALLBACK_H_
