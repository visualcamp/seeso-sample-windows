#ifndef SEESO_FRAMWORK_CORE_CALLBACK_H_
#define SEESO_FRAMWORK_CORE_CALLBACK_H_

#include "seeso/framework/callback_dispatcher.h"
#include "seeso/framework/values.h"
#include "seeso/callback/igaze_callback.h"
#include "seeso/callback/icalibration_callback.h"
#include "seeso/callback/istatus_callback.h"
#include "seeso/callback/iface_callback.h"

#include <memory>
#include <cstdint>

namespace seeso {


class CoreCallback final : private internal::CallbackDispatcher<CoreCallback> {
 public:
  CoreCallback();

  void OnGaze(uint64_t timestamp,
              float x, float y,
              float fixation_x, float fixation_y,
              int movement_duration,
              seeso::TrackingState tracking_state,
              seeso::EyeMovementState eye_movement_state);

  void OnStatus(uint64_t timestamp, std::vector<float> data);
  void OnFace(uint64_t timestamp, std::vector<float> data);

  void OnCalibrationProgress(float progress);
  void OnCalibrationNextPoint(float next_point_x, float next_point_y);
  void OnCalibrationFinished(std::vector<float> calib_data);

  void setStatusOptions(const std::vector<int>& statusOptions);

  void setGazeCallback(IGazeCallback* callback);
  void setCalibrationCallback(ICalibrationCallback* callback);
  void setStatusCallback(IStatusCallback* callback);
//  void setFaceCallback(seeso::IFaceCallback* callback);

private:
  IGazeCallback* gaze_callback;
  ICalibrationCallback* calibration_callback;
  IStatusCallback* status_callback;
  IFaceCallback* face_callback;

  int isUseAttention = false;
  int isUseBlink = false;
  int isUseDrowsiness = false;
};

} // namespace seeso

#endif //SEESO_FRAMWORK_CORE_CALLBACK_H_
