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
   * @param fixation_x          fixation x-axis
   * @param fixation_y          fixation y-axis
   * @param tracking_state      tracking state
   * @param eye_movement_state  eye movement state
   */
  virtual void OnGaze(uint64_t timestamp, float x, float y, float fixation_x, float fixation_y,
                      TrackingState tracking_state, EyeMovementState eye_movement_state) = 0;

 /** Status Callback
 *
 * @param version             version info
 * @param timestamp           timestamp (passed by EyeTracker::AddFrame())
 * @param data                status data vector
 *                                eye movement = data[0];
 *                                fixation = data[1];
 *                                EAR = data[2];
 *                                drowsiness = data[3];
 *                                ECR = data[4];
 *                                blink = data[5];
 *                                attention = data[6];
 *                                EAR Left = data[7];
 *                                EAR Right = data[8];
 *                                blink Right = data[9];
 *                                blink Left = data[10];
 */
  virtual void OnStatus(int32_t version, uint64_t timestamp, std::vector<float>& data) = 0;

  /** Face Callback
 *
 * @param version             version info
 * @param timestamp           timestamp (passed by EyeTracker::AddFrame())
 * @param data                face data vector
 */
  virtual void OnFace(int32_t version, uint64_t timestamp, std::vector<float>& data) = 0;

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
