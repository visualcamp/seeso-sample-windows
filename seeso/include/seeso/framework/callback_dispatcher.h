//
// Created by cosge on 2021-03-26.
//

#ifndef SEESO_FRAMEWORK_CALLBACK_DISPATCHER_H_
#define SEESO_FRAMEWORK_CALLBACK_DISPATCHER_H_

#include <cstdint>
#include <vector>

#include "seeso/framework/values.h"

namespace seeso {
namespace internal {

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

  static void
  dispatchOnGaze(derived_type *obj, uint64_t timestamp,
                 float x, float y,
                 float fixation_x, float fixation_y,
                 int32_t movement_duration,
                 int32_t tracking_state, int32_t eye_movement_state);

  static void
  dispatchOnStatus(derived_type *obj, int32_t version, uint64_t timestamp, const float* data, uint32_t data_size);

  static void
  dispatchOnFace(derived_type *obj, int32_t version, uint64_t timestamp, const float* data, uint32_t data_size);

  static void dispatchOnCalibrationProgress(derived_type *obj, float progress);

  static void dispatchOnCalibrationNextPoint(derived_type *obj, float next_point_x, float next_point_y);

  static void dispatchOnCalibrationFinished(derived_type *obj, const float* data, uint32_t data_size);

};

template<typename Derived>
void CallbackDispatcher<Derived>::dispatchOnGaze(derived_type* obj,
                                                 uint64_t timestamp,
                                                 float x, float y,
                                                 float fixation_x, float fixation_y,
                                                 int32_t movement_duration,
                                                 int32_t tracking_state, int32_t eye_movement_state) {
  static auto mfptr = &derived_type::OnGaze;
  (obj->*mfptr)(timestamp,
      x, y,
      fixation_x, fixation_y,
      movement_duration,
      static_cast<TrackingState>(tracking_state),
      static_cast<EyeMovementState>(eye_movement_state));
}


template<typename Derived>
void
CallbackDispatcher<Derived>::dispatchOnStatus(derived_type* obj, int32_t version, uint64_t timestamp, const float* data,
                                              uint32_t data_size) {
  static auto mfptr = &derived_type::OnStatus;
  (obj->*mfptr)(timestamp, {data, data + data_size});
}

template<typename Derived>
void
CallbackDispatcher<Derived>::dispatchOnFace(derived_type *obj, int32_t version, uint64_t timestamp, const float* data,
                                            uint32_t data_size) {
  static auto mfptr = &derived_type::OnFace;
  (obj->*mfptr)(timestamp, {data, data + data_size});
}

template<typename Derived>
void CallbackDispatcher<Derived>::dispatchOnCalibrationProgress(derived_type* obj,
                                                                float progress) {
  static auto mfptr = &derived_type::OnCalibrationProgress;
  (obj->*mfptr)(progress);
}

template<typename Derived>
void CallbackDispatcher<Derived>::dispatchOnCalibrationNextPoint(derived_type* obj,
                                                                 float next_point_x, float next_point_y) {
  static auto mfptr = &derived_type::OnCalibrationNextPoint;
  (obj->*mfptr)(next_point_x, next_point_y);
}

template<typename Derived>
void CallbackDispatcher<Derived>::dispatchOnCalibrationFinished(derived_type* obj,
                                                                const float* data, uint32_t data_size) {
  static auto mfptr = &derived_type::OnCalibrationFinished;
  (obj->*mfptr)({data, data + data_size});
}

// template helper for dispatcher function to pass through C API
template<typename T>
struct dispatch_c;

template<typename R, typename Object, typename ...Args>
struct dispatch_c<R(*)(Object *, Args...)> {
  using type = std::add_pointer_t<R(void *, Args...)>;
};

template<typename T>
using dispatch_c_t = typename dispatch_c<T>::type;

template<typename T>
constexpr inline
dispatch_c_t<T> make_dispatch_c(T fptr) {
  return (dispatch_c_t<T>) fptr;
}

} // namespace internal
} // namespace seeso

#endif //SEESO_FRAMEWORK_CALLBACK_DISPATCHER_H_
