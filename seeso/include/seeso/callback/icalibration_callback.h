//
// Created by cosge on 2021-04-27.
//

#ifndef SEESO_CALLBACK_ICALIBRATION_CALLBACK_H_
#define SEESO_CALLBACK_ICALIBRATION_CALLBACK_H_

#include <vector>

namespace seeso {

class ICalibrationCallback {
 protected:
  ICalibrationCallback(const ICalibrationCallback&) = default;
  ICalibrationCallback& operator = (const ICalibrationCallback&) = default;

 public:
  ICalibrationCallback() = default;
  virtual ~ICalibrationCallback() = default;

  /** Calibration Progress Callback
   * @brief called during each calibration point(s)
   *
   * @param progress    progress(0.0 ~ 1.0)
   */
  virtual void OnCalibrationProgress(float progress) = 0;

  /** Next Point Callback
   * @brief called when calibration point changes
   *
   * @param x    new point x
   * @param y    new point y
   */
  virtual void OnCalibrationNextPoint(float x, float y) = 0;

  /** Calibration Finished Callback
   * @brief called when calibration is finished
   *
   * @param data      calibration data.
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
  virtual void OnCalibrationFinish(const std::vector<float>& data) = 0;
};

}

#endif //SEESO_CALLBACK_ICALIBRATION_CALLBACK_H_
