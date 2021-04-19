//
// Created by cosge on 2021-03-24.
//

#include "eye_tracker.h"
#include <string>
#include <vector>
#include <windows.h>

#define SET_DLL_FUNCTION(hinst, name) \
d##name.setFuncPtr(GetProcAddress(hinst, #name))

#define SET_DLL_SEESO_FUNCTION(hinst, name) \
d##name.setFuncPtr(GetProcAddress(hinst, "SeeSo"#name))

namespace seeso {

std::string EyeTracker::getVersion() const {
  return std::string(dGetVersion());
}

EyeTracker::EyeTracker(HINSTANCE procIDDLL)
    : wrapper(nullptr)
{
  SET_DLL_FUNCTION(procIDDLL, CreateSeeSo);
  SET_DLL_FUNCTION(procIDDLL, DeleteSeeSo);
  SET_DLL_FUNCTION(procIDDLL, GetVersion);

  SET_DLL_SEESO_FUNCTION(procIDDLL, DeinitEyeTracker);
  SET_DLL_SEESO_FUNCTION(procIDDLL, SetTrackingFps);
  SET_DLL_SEESO_FUNCTION(procIDDLL, SetCameraDistanceZ);
  SET_DLL_SEESO_FUNCTION(procIDDLL, SetCalibrationRegion);
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
  wrapper = dCreateSeeSo(license_key.c_str(), license_key.size(),
                         3.14f/4, /* camera fov */
                         3,       /* thread num */
                         0,        /* use GPU(Not supported on Windows) */
                         statusOptions.data(), static_cast<int>(statusOptions.size()));
  auto internal_code = dGetAuthorizationResult(wrapper);
  if(internal_code != 0)
    return internal_code + 2;
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
  dSetCameraDistanceZ(wrapper, static_cast<float>(face_distance_mm));
}

int EyeTracker::getFaceDistance() const {
  return face_distance_mm / 10;
}

void EyeTracker::startCalibration(TargetNum num, CalibrationAccuracy criteria,
                                   float left, float top, float right, float bottom) {
  dSetCalibrationRegion(wrapper, left, top, right, bottom);
  dStartCalibration(wrapper, num, static_cast<int>(criteria));
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
  using dispatcher_type = CallbackDispatcher<CallbackInterface>;

  dSetCallbackInterface(
      wrapper, callback_obj,
      (void (*)(void *, uint64_t, float, float, float, float, int32_t, int32_t))&dispatcher_type::dispatchOnGaze,
      (void (*)(void *, int32_t, uint64_t, const float*, int32_t))&dispatcher_type::dispatchOnStatus,
      (void (*)(void *, int32_t, uint64_t, const float*, int32_t))&dispatcher_type::dispatchOnFace,
      (void (*)(void *, float, float))&dispatcher_type::dispatchOnCalibrationNextPoint,
      (void (*)(void *, float))&dispatcher_type::dispatchOnCalibrationProgress,
      (void (*)(void *, const float *, int32_t))&dispatcher_type::dispatchOnCalibrationFinished);
}

void EyeTracker::removeCallbackInterface() {
  dRemoveCallbackInterface(wrapper);
}

bool EyeTracker::addFrame(int64_t time_stamp, uint8_t *buffer, int width, int height) {
  return dAddFrame(wrapper, time_stamp, buffer, width, height);
}

} // namespace seeso
