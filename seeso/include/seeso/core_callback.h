/**
 * Written by Dean on 2021-04-07.
 * When EyeTracker class is initialized normally, the overloaded functions of the 'Callback' class are called in time.
 * */

#ifndef SEESO_WINDOWS_CORE_CALLBACK_H
#define SEESO_WINDOWS_CORE_CALLBACK_H

#include <memory>
#include "seeso/callback_interface.h"
#include "seeso/core_callback_interface.h"
#include "seeso/values.h"
#include "seeso/util/coord_converter.h"

class CoreCallback : public seeso::CoreCallbackInterface {
 public:
  explicit CoreCallback();

  void OnGaze(uint64_t timestamp, float x, float y, float fixation_x, float fixation_y,
    seeso::TrackingState tracking_state, seeso::EyeMovementState eye_movement_state) override;

  void OnStatus(uint64_t timestamp, std::vector<float> data) override;
  void OnFace(uint64_t timestamp, std::vector<float> data) override;

  void OnCalibrationProgress(float progress) override;
  void OnCalibrationNextPoint(float next_point_x, float next_point_y) override;
  void OnCalibrationFinished(std::vector<float> calib_data) override;

  void setStatusOptions(const std::vector<int>& statusOptions);
  void setCallbackInterface(seeso::CallbackInterface* callback_obj);
  void removeCallbackInterface();

private:
  seeso::CallbackInterface* callback = nullptr;
  int isUseAttention = false;
  int isUseBlink = false;
  int isUseDrowsiness = false;
};


#endif //SEESO_WINDOWS_CORE_CALLBACK_H
