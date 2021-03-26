//
// Created by YongGyu Lee on 2021-03-24.
//

#ifndef SEESO_DLL_FUNCTION_H_
#define SEESO_DLL_FUNCTION_H_

#include <type_traits>
#include <windows.h>

namespace seeso {

template<typename T> struct DFunction;

/** @brief dll function wrapper
 *
 * @tparam R        return type
 * @tparam Args     argument types
 */
template<typename R, typename ...Args>
class DFunction<R(Args...)> {
 public:
  using return_type = R;
  using fptr_type = std::add_pointer_t<R WINAPI(Args...)>;

  return_type inline operator()(Args... args)       { return fptr(args...); }
  return_type inline operator()(Args... args) const { return fptr(args...); }

  void inline setFuncPtr(fptr_type ptr) { fptr = ptr;             }
  void inline setFuncPtr(FARPROC ptr)   { fptr = (fptr_type)ptr;  }

 private:
  fptr_type fptr = nullptr;
};

}

#endif //SEESO_DLL_FUNCTION_H_
