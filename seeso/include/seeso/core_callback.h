/**
 * Written by Dean on 2021-04-07.
 * When EyeTracker class is initialized normally, the overloaded functions of the 'Callback' class are called in time.
 * */

#ifndef SEESO_WINDOWS_CORE_CALLBACK_H
#define SEESO_WINDOWS_CORE_CALLBACK_H

#include <memory>
#include <cstdint>
#include "seeso/callback_dispatcher.h"
#include "seeso/callback_interface.h"
#include "seeso/values.h"

namespace seeso {


class CoreCallback : private internal::CallbackDispatcher<CoreCallback> {
 public:
  void OnGaze(uint64_t timestamp, float x, float y, float fixation_x, float fixation_y,
    int movement_duration, seeso::TrackingState tracking_state, seeso::EyeMovementState eye_movement_state);

  void OnStatus(uint64_t timestamp, std::vector<float> data);
  void OnFace(uint64_t timestamp, std::vector<float> data);

  void OnCalibrationProgress(float progress);
  void OnCalibrationNextPoint(float next_point_x, float next_point_y);
  void OnCalibrationFinished(std::vector<float> calib_data);

  void setStatusOptions(const std::vector<int>& statusOptions);
  void setCallbackInterface(seeso::CallbackInterface* callback_obj);
  void removeCallbackInterface();

private:
  seeso::CallbackInterface* callback = nullptr;
  int isUseAttention = false;
  int isUseBlink = false;
  int isUseDrowsiness = false;
};

} // namespace seeso

#endif //SEESO_WINDOWS_CORE_CALLBACK_H
