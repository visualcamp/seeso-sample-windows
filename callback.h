/**
 * Written by Dean on 2021-04-07.
 * When EyeTracker class is initialized normally, the overloaded functions of the 'Callback' class are called in time.
 * */

#ifndef SEESO_WINDOWS_CALLBACK_H
#define SEESO_WINDOWS_CALLBACK_H

#include <memory>
#include "seeso/callback_interface.h"
#include "seeso/util/display.h"
#include "seeso/util/coord_converter.h"
#include "view.h"

class Callback : public seeso::CallbackInterface {
 public:
  explicit Callback(const seeso::DisplayInfo &display);

  void OnGaze(uint64_t timestamp, float x, float y, float fixation_x, float fixation_y,
              seeso::TrackingState tracking_state, seeso::EyeMovementState eye_movement_state) override;

  void OnStatus(int32_t version, uint64_t timestamp, std::vector<float> data) override;
  void OnFace(int32_t version, uint64_t timestamp, std::vector<float> data) override;

  /*
  void OnAttention(float attention) override;
  void OnBlink(bool isBlinkLeft, bool isBlinkRight, bool isBlink, float eyeOpenness) override;
  void OnDrowsiness(bool isDrowsiness) override;
  */

  void OnCalibrationProgress(float progress) override;
  void OnCalibrationNextPoint(float next_point_x, float next_point_y) override;
  void OnCalibrationFinished(std::vector<float> calib_data) override;
  void registerView(const std::shared_ptr<seeso::View> &view);

 private:
  std::shared_ptr<seeso::View> view;
  seeso::CoordConverter pc;
  seeso::WindowsDisplayUtil wdu;
};


#endif //SEESO_WINDOWS_CALLBACK_H
