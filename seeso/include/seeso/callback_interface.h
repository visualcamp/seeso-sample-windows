//
// Created by cosge on 2021-03-25.
//

#ifndef SEESO_WINDOWS_CALLBACK_INTERFACE_H
#define SEESO_WINDOWS_CALLBACK_INTERFACE_H

#include <cstdint>
#include "seeso/callback_dispatcher.h"
#include "seeso/values.h"

namespace seeso {

class CallbackInterface : private CallbackDispatcher<CallbackInterface> {
 public:
  virtual ~CallbackInterface() = default;

  /** Gaze Callback
   *
   * @param timestamp           timestamp (passed by EyeTracker::AddFrame())
   * @param x                   mm in x-axis (camera coordinate)
   * @param y                   mm in y-axis (camera coordinate)
   * @param tracking_state      tracking state
   * @param eye_movement_state  eye movement state
   */
  virtual void OnGaze(uint64_t timestamp, float x, float y,
                      TrackingState tracking_state, EyeMovementState eye_movement_state) = 0;

  /** Calibration Progress Callback
   * @brief called during each calibration process
   *
   * @param progress    progress(0.0 ~ 1.0)  per points
   */
  virtual void OnCalibrationProgress(float progress) = 0;

  /** Next Point Callback
   * @brief called when calibration point changes
   *
   * @param next_point_x    changed point x
   * @param next_point_y    changed point y
   */
  virtual void OnCalibrationNextPoint(float next_point_x, float next_point_y) = 0;

  /** Calibration Finished Callback
   * @brief called when calibration is finished
   *
   * @param calib_data      calibration data.
   *
   * This value can be stored to set calibration data on other seeso::EyeTracker object.
   * You don't need to pass data to the same seeso::EyeTracker object.
   *
   * @code{.cpp}
   *
   * seeso::EyeTracker e1;
   * auto cb = new Callback(); // inherited from seeso::CallbackInterface
   * e1.setCallbackInterface(cb);
   * e1.startCalibration(...);
   *
   * // assume that calibration for e1 is done.
   * // assume that calibration data can be got from cb
   *
   * seeso::EyeTracker e2;
   * e2.setCalibrationData(cb.getData()); // no need to calibrate again!
   *
   * @endcode
   */
  virtual void OnCalibrationFinished(std::vector<float> calib_data) = 0;
};

} // namespace seeso

#endif //SEESO_WINDOWS_CALLBACK_INTERFACE_H
