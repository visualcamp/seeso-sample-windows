//
// Created by cosge on 2021-03-24.
//

#include "seeso/eye_tracker.h"
#include "seeso/util/dll_function.h"

#include <string>
#include <vector>
#include <iostream>
#include <stdexcept>

#include <windows.h>

#include "seeso/framework/dll_export.h"

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
  SET_DLL_FUNCTION_IMPL(hinst, dll::name, #name)

#define SET_DLL_SEESO_FUNCTION(hinst, name) \
  SET_DLL_FUNCTION_IMPL(hinst, dll::SeeSo##name, "SeeSo" #name)

#define CREATE_DLL_FUNC(name) \
  DLLFunction<decltype(name)> d##name

#define CAST_OBJ(ptr) static_cast<SeeSoObject*>(ptr)

namespace seeso {

namespace dll {
namespace {

DLLFunction<decltype(GetSeeSoVersionString)>        GetSeeSoVersionString;
DLLFunction<decltype(GetSeeSoVersionNumber)>        GetSeeSoVersionNumber;

DLLFunction<decltype(CreateSeeSo)>                  CreateSeeSo;
DLLFunction<decltype(DeleteSeeSo)>                  DeleteSeeSo;

DLLFunction<decltype(SeeSoInitEyeTracker)>          SeeSoInitEyeTracker;
DLLFunction<decltype(SeeSoDeinitEyeTracker)>        SeeSoDeinitEyeTracker;
DLLFunction<decltype(SeeSoSetTrackingFps)>          SeeSoSetTrackingFps;
DLLFunction<decltype(SeeSoSetFaceDistance)>         SeeSoSetFaceDistance;
DLLFunction<decltype(SeeSoSetFixationCount)>        SeeSoSetFixationCount;
DLLFunction<decltype(SeeSoSetFilterType)>           SeeSoSetFilterType;
DLLFunction<decltype(SeeSoStartCalibration)>        SeeSoStartCalibration;
DLLFunction<decltype(SeeSoStartCollectSamples)>     SeeSoStartCollectSamples;
DLLFunction<decltype(SeeSoStopCalibration)>         SeeSoStopCalibration;
DLLFunction<decltype(SeeSoSetCalibrationData)>      SeeSoSetCalibrationData;
DLLFunction<decltype(SeeSoSetCallbackInterface)>    SeeSoSetCallbackInterface;
DLLFunction<decltype(SeeSoRemoveCallbackInterface)> SeeSoRemoveCallbackInterface;
DLLFunction<decltype(SeeSoAddFrame)>                SeeSoAddFrame;
DLLFunction<decltype(SeeSoGetAuthorizationResult)>  SeeSoGetAuthorizationResult;

}
} // namespace dll

void global_init(const char* file) {

  auto procIDDLL = LoadLibrary(file);
  if(procIDDLL == NULL) {
    std::cerr << "Failed to load " << file << '\n';
    throw std::runtime_error(std::string("Failed to load ") + file);
  }

  SET_DLL_FUNCTION(procIDDLL, CreateSeeSo);
  SET_DLL_FUNCTION(procIDDLL, DeleteSeeSo);
  SET_DLL_FUNCTION(procIDDLL, GetSeeSoVersionString);
  SET_DLL_FUNCTION(procIDDLL, GetSeeSoVersionNumber);

  SET_DLL_SEESO_FUNCTION(procIDDLL, InitEyeTracker);
  SET_DLL_SEESO_FUNCTION(procIDDLL, DeinitEyeTracker);
  SET_DLL_SEESO_FUNCTION(procIDDLL, SetTrackingFps);
  SET_DLL_SEESO_FUNCTION(procIDDLL, SetFaceDistance);

  SET_DLL_SEESO_FUNCTION(procIDDLL, StartCalibration);
  SET_DLL_SEESO_FUNCTION(procIDDLL, StartCollectSamples);
  SET_DLL_SEESO_FUNCTION(procIDDLL, StopCalibration);
  SET_DLL_SEESO_FUNCTION(procIDDLL, SetCalibrationData);
  SET_DLL_SEESO_FUNCTION(procIDDLL, SetCallbackInterface);
  SET_DLL_SEESO_FUNCTION(procIDDLL, RemoveCallbackInterface);
  SET_DLL_SEESO_FUNCTION(procIDDLL, AddFrame);
  SET_DLL_SEESO_FUNCTION(procIDDLL, GetAuthorizationResult);

  static_assert(std::is_same<decltype(CreateSeeSo), SeeSoObject*(const char*, uint32_t)>::value, "");
}

std::string getVersionStr() {
  return dll::GetSeeSoVersionString();
}

int32_t getVersionInt() {
  return dll::GetSeeSoVersionNumber();
}


// EyeTracker

int EyeTracker::initialize(const std::string& license_key, const UserStatusOption& options) {
  if(seeso_object == nullptr) {
    seeso_object = dll::CreateSeeSo(license_key.c_str(), static_cast<uint32_t>(license_key.size()));
  }

  std::vector<int32_t> statusOptions;
  if (options.getUseAttention()) {
    statusOptions.push_back(StatusOptions::STATUS_ATTENTION);
  }
  if (options.getUseBlink()) {
    statusOptions.push_back(StatusOptions::STATUS_BLINK);
  }
  if (options.getUseDrowsiness()) {
    statusOptions.push_back(StatusOptions::STATUS_DROWSINESS);
  }

  auto internal_code = dll::SeeSoGetAuthorizationResult(CAST_OBJ(seeso_object));
  if(internal_code != 0)
    return internal_code + 2;
  else {
    dll::SeeSoInitEyeTracker(
        CAST_OBJ(seeso_object),
        3.14f / 4.f,
        3,
        0,
        statusOptions.data(), static_cast<uint32_t>(statusOptions.size()));

    callback = CoreCallback();
    callback.setStatusOptions(statusOptions);

    using dispatcher = internal::CallbackDispatcher<CoreCallback>;
    dll::SeeSoSetCallbackInterface(
        CAST_OBJ(seeso_object), &callback,
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
  dll::SeeSoDeinitEyeTracker(CAST_OBJ(seeso_object));
}

EyeTracker::~EyeTracker() {
  dll::DeleteSeeSo(CAST_OBJ(seeso_object));
  seeso_object = nullptr;
}

void EyeTracker::setTrackingFps(int fps) {
  dll::SeeSoSetTrackingFps(CAST_OBJ(seeso_object), fps);
}

void EyeTracker::setFaceDistance(int cm) {
  face_distance_mm = cm * 10;
  dll::SeeSoSetFaceDistance(CAST_OBJ(seeso_object), face_distance_mm);
}

int EyeTracker::getFaceDistance() const {
  return face_distance_mm / 10;
}

void EyeTracker::startCalibration(TargetNum num, CalibrationAccuracy criteria,
                                   float left, float top, float right, float bottom) {
  dll::SeeSoStartCalibration(CAST_OBJ(seeso_object), static_cast<int32_t>(num), static_cast<int32_t>(criteria), left, top, right, bottom);
}

void EyeTracker::startCollectSamples() {
  dll::SeeSoStartCollectSamples(CAST_OBJ(seeso_object));
}

void EyeTracker::stopCalibration() {
  dll::SeeSoStopCalibration(CAST_OBJ(seeso_object));
}

void EyeTracker::setCalibrationData(const std::vector<float> &serialData) {
  dll::SeeSoSetCalibrationData(CAST_OBJ(seeso_object), serialData.data(), static_cast<uint32_t>(serialData.size() * sizeof(float)));
}

void EyeTracker::removeCallbackInterface() {
  dll::SeeSoRemoveCallbackInterface(CAST_OBJ(seeso_object));
}

bool EyeTracker::addFrame(int64_t time_stamp, uint8_t *buffer, int width, int height) {
  return dll::SeeSoAddFrame(CAST_OBJ(seeso_object), time_stamp, buffer, width, height);
}

void EyeTracker::setGazeCallback(seeso::IGazeCallback *listener) {
  callback.setGazeCallback(listener);
}

void EyeTracker::setCalibrationCallback(seeso::ICalibrationCallback *listener) {
  callback.setCalibrationCallback(listener);
}

void EyeTracker::setStatusCallback(seeso::IStatusCallback *listener) {
  callback.setStatusCallback(listener);
}

} // namespace seeso
