//
// Created by cosge on 2021-03-24.
//

#include "seeso/eye_tracker.h"

#include <string>
#include <vector>
#include <stdexcept>
#include <iostream>

#include <windows.h>

#define SET_DLL_FUNCTION_IMPL(hinst, target, name)  \
do {                                                \
  target.setFuncPtr(GetProcAddress(hinst, name));   \
  if(target == nullptr) {                           \
    std::cerr                                       \
      << "Failed to find " name " from dll\n";      \
    throw std::runtime_error(                       \
      "Failed to find " name " from dll\n");        \
  }                                                 \
} while(false)

#define SET_DLL_FUNCTION(hinst, name) \
  SET_DLL_FUNCTION_IMPL(hinst, d##name, #name)

#define SET_DLL_SEESO_FUNCTION(hinst, name) \
  SET_DLL_FUNCTION_IMPL(hinst, d##name, "SeeSo" #name)

namespace seeso {

std::string EyeTracker::getVersion() const {
  return dGetVersion();
}

EyeTracker::EyeTracker(HINSTANCE procIDDLL)
{
  SET_DLL_FUNCTION(procIDDLL, CreateSeeSo);
  SET_DLL_FUNCTION(procIDDLL, DeleteSeeSo);
  SET_DLL_FUNCTION(procIDDLL, GetVersion);

  SET_DLL_SEESO_FUNCTION(procIDDLL, InitEyeTracker);
  SET_DLL_SEESO_FUNCTION(procIDDLL, DeinitEyeTracker);
  SET_DLL_SEESO_FUNCTION(procIDDLL, SetTrackingFps);
  SET_DLL_SEESO_FUNCTION(procIDDLL, SetFaceDistance);
//  SET_DLL_SEESO_FUNCTION(procIDDLL, SetCalibrationRegion);
  SET_DLL_SEESO_FUNCTION(procIDDLL, StartCalibration);
  SET_DLL_SEESO_FUNCTION(procIDDLL, StartCollectSamples);
  SET_DLL_SEESO_FUNCTION(procIDDLL, StopCalibration);
  SET_DLL_SEESO_FUNCTION(procIDDLL, SetCalibrationData);
  SET_DLL_SEESO_FUNCTION(procIDDLL, SetCallbackInterface);
  SET_DLL_SEESO_FUNCTION(procIDDLL, RemoveCallbackInterface);
  SET_DLL_SEESO_FUNCTION(procIDDLL, AddFrame);
  SET_DLL_SEESO_FUNCTION(procIDDLL, GetAuthorizationResult);
}

int EyeTracker::initialize(const std::string& license_key, const std::vector<int> &statusOptions) {
  if(wrapper == nullptr) {
    wrapper = dCreateSeeSo(license_key.c_str(), license_key.size());
  }

  auto internal_code = dGetAuthorizationResult(wrapper);
  if (internal_code != 0) {
    return internal_code + 2;
  }
  else {
    dInitEyeTracker(wrapper, (3.14f / 4), /* camera fov */
                    3,       /* thread num */
                    0,        /* use GPU(Not supported on Windows) */
                    statusOptions.data(), static_cast<int>(statusOptions.size()));

    callback = CoreCallback();
    callback.setStatusOptions(statusOptions);

    dSetCallbackInterface(
        wrapper, &callback,
        internal::make_dispatch_c(&dispatcher::dispatchOnGaze),
        internal::make_dispatch_c(&dispatcher::dispatchOnStatus),
        internal::make_dispatch_c(&dispatcher::dispatchOnFace),
        internal::make_dispatch_c(&dispatcher::dispatchOnCalibrationNextPoint),
        internal::make_dispatch_c(&dispatcher::dispatchOnCalibrationProgress),
        internal::make_dispatch_c(&dispatcher::dispatchOnCalibrationFinished));
  }
  return 0;
}

void EyeTracker::deinitialize() {
  dDeinitEyeTracker(wrapper);
}

EyeTracker::~EyeTracker() {
  dDeleteSeeSo(wrapper);
}

void EyeTracker::setTrackingFps(int fps) {
  dSetTrackingFps(wrapper, fps);
}

void EyeTracker::setFaceDistance(int cm) {
  face_distance_mm = cm * 10;
  dSetFaceDistance(wrapper, static_cast<float>(face_distance_mm));
}

int EyeTracker::getFaceDistance() const {
  return face_distance_mm / 10;
}

void EyeTracker::startCalibration(TargetNum num, CalibrationAccuracy criteria,
                                   float left, float top, float right, float bottom) {
  dStartCalibration(wrapper, num, static_cast<int>(criteria), left, top, right, bottom);
}

void EyeTracker::startCollectSamples() {
  dStartCollectSamples(wrapper);
}

void EyeTracker::stopCalibration() {
  dStopCalibration(wrapper);
}

void EyeTracker::setCalibrationData(const std::vector<float> &serialData) {
  dSetCalibrationData(wrapper, serialData.data(), serialData.size() * sizeof(float));
}

void EyeTracker::setCallbackInterface(CallbackInterface *callback_obj) {
  callback.setCallbackInterface(callback_obj);
}

void EyeTracker::removeCallbackInterface() {
  dRemoveCallbackInterface(wrapper);
  callback;
}

bool EyeTracker::addFrame(int64_t time_stamp, uint8_t *buffer, int width, int height) {
  return dAddFrame(wrapper, time_stamp, buffer, width, height);
}

} // namespace seeso
