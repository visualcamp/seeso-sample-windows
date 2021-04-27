//
// Created by cosge on 2021-04-27.
//

#ifndef SEESO_CALLBACK_ISTATUS_CALLBACK_H_
#define SEESO_CALLBACK_ISTATUS_CALLBACK_H_

#include <cstdint>

namespace seeso {

class IStatusCallback {
 protected:
  IStatusCallback(const IStatusCallback&) = default;
  IStatusCallback& operator = (const IStatusCallback&) = default;

 public:
  IStatusCallback() = default;
  virtual ~IStatusCallback() = default;


  /** Attention Status Callback
   *
   * @param timestamp          timestamp (passed by EyeTracker::AddFrame())
   * @param score              Attention rate score (0.0f ~ 1.0f)
   */
  virtual void OnAttention(uint64_t timestamp, float score) = 0;

  /** Drowsiness Status Callback
   *
   * @param timestamp           timestamp (passed by EyeTracker::AddFrame())
   * @param isDrowsiness        Drowsiness flag (True/False)
   */
  virtual void OnDrowsiness(uint64_t timestamp, bool isDrowsiness) = 0;

  /** Blink Statis Callback
   *
   * @param timestamp           timestamp (passed by EyeTracker::AddFrame())
   * @param isBlinkLeft         Left Eye Blink flag (True/False)
   * @param isBlinkRight        Right Eye Blink flag (True/False)
   * @param isBlink             Eye Blink flag (True/False)
   * @param eyeOpenness         Eye Openness rate (0.0f ~ 1.0f)
   */
  virtual void OnBlink(uint64_t timestamp, bool isBlinkLeft, bool isBlinkRight, bool isBlink, float eyeOpenness) = 0;
};

} // namespace seeso

#endif //SEESO_CALLBACK_ISTATUS_CALLBACK_H_
