//
// Created by cosge on 2021-03-26.
//

#ifndef SEESO_WINDOWS_CALLBACK_DISPATCHER_H
#define SEESO_WINDOWS_CALLBACK_DISPATCHER_H

#include <cstdint>
#include <vector>
#include "seeso/values.h"

namespace seeso {

/** Callback Object Dispatcher
 * @brief callback method dispatcher
 *
 * instead of CallbackInterface's virtual method, this class' static methods are passed to internal SeeSo object.
 * (since member function pointer cannot be expressed to C-style function pointers, and to avoid different vtable ABI)
 * Dispatcher function is called from internal SeeSo object,
 * and then the dispatcher functions calls CallbackInterface object's methods.
 *
 * @tparam Derived      Interface type
 */
template<typename Derived>
class CallbackDispatcher {
 public:
  using derived_type = Derived;

  static void dispatchOnGaze(derived_type *obj, uint64_t timestamp, float x, float y, float fixation_x, float fixation_y,
                             int tracking_state, int eye_movement_state);
  static void dispatchOnStatus(derived_type* obj, int32_t version, uint64_t timestamp, std::vector<float>& data);
  static void dispatchOnFace(derived_type* obj, int32_t version, uint64_t timestamp, std::vector<float>& data);
  static void dispatchOnCalibrationProgress(derived_type *obj, float progress);
  static void dispatchOnCalibrationNextPoint(derived_type *obj, float next_point_x, float next_point_y);
  static void dispatchOnCalibrationFinished(derived_type *obj, std::vector<float>& data);

};

template<typename Derived>
void CallbackDispatcher<Derived>::dispatchOnGaze(CallbackDispatcher<Derived>::derived_type* obj,
                                                 uint64_t timestamp, float x, float y,
                                                 float fixation_x, float fixation_y,
                                                 int tracking_state, int eye_movement_state) {
  static auto mfptr = &derived_type::OnGaze;
  (obj->*mfptr)(timestamp, x, y, fixation_x, fixation_y, static_cast<TrackingState>(tracking_state), static_cast<EyeMovementState>(eye_movement_state));
}


template<typename Derived>
void CallbackDispatcher<Derived>::dispatchOnStatus(derived_type* obj, int32_t version, uint64_t timestamp, std::vector<float>& data) {
  static auto mfptr = &derived_type::OnStatus;
  (obj->*mfptr)(version, timestamp, data);
}

template<typename Derived>
void CallbackDispatcher<Derived>::dispatchOnFace(derived_type* obj, int32_t version, uint64_t timestamp, std::vector<float>& data) {
  static auto mfptr = &derived_type::OnFace;
  (obj->*mfptr)(version, timestamp, data);
}

template<typename Derived>
void CallbackDispatcher<Derived>::dispatchOnCalibrationProgress(CallbackDispatcher<Derived>::derived_type* obj,
                                                                float progress) {
  static auto mfptr = &derived_type::OnCalibrationProgress;
  (obj->*mfptr)(progress);
}

template<typename Derived>
void CallbackDispatcher<Derived>::dispatchOnCalibrationNextPoint(CallbackDispatcher<Derived>::derived_type* obj,
                                                                 float next_point_x, float next_point_y) {
  static auto mfptr = &derived_type::OnCalibrationNextPoint;
  (obj->*mfptr)(next_point_x, next_point_y);
}

template<typename Derived>
void CallbackDispatcher<Derived>::dispatchOnCalibrationFinished(CallbackDispatcher<Derived>::derived_type* obj,
                                                                std::vector<float>& data) {
  static auto mfptr = &derived_type::OnCalibrationFinished;
  (obj->*mfptr)(data);
}

}

#endif //SEESO_WINDOWS_CALLBACK_DISPATCHER_H
