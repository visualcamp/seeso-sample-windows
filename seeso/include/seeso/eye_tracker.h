//
// Created by cosge on 2021-03-24.
//

#include <vector>
#include <string>
#include "seeso/dll_function.h"

//#include "window-callback.h"

#ifndef UNTITLED1_EYE_TRACKER_H
#define UNTITLED1_EYE_TRACKER_H

#include <string>
#include <vector>
#include "seeso/callback_interface.h"
#include "seeso/core_callback.h"
#include "seeso/values.h"

namespace seeso {


class EyeTracker {
 public:

  std::string getVersion() const;

  // load DLL library
  explicit EyeTracker(HINSTANCE procIDDLL);
  ~EyeTracker();

  // init SeeSo object
  int initialize(const std::string& license_key, const std::vector<int> &statusOptions);
  // destroy SeeSo object
  void deinitialize();

  // set the distance between face and camera
  void setFaceDistance(int cm);
  int getFaceDistance() const; // return cm

  // set listener
  void setCallbackInterface(CallbackInterface* callback_obj);

  // remove listener
  void removeCallbackInterface();

  // set maximum fps
  void setTrackingFps(int fps);

  // Image
  /**
   *
   * @param time_stamp      frame timestamp(passed out to callback)
   * @param buffer          frame data
   * @param width           frame width
   * @param height          frame height
   * @return
   *    frame may be omitted if there's no idle thread.
   *    Then this returns false. Else true
   */
  bool addFrame(int64_t time_stamp, uint8_t *buffer, int width, int height);

  // Calibration
  void startCalibration(TargetNum num, CalibrationAccuracy criteria,
                         float left, float top, float right, float bottom);
//  void setCalibrationRegion(float left, float top, float right, float bottom);
//  void startCalibration(int mode, int criteria);
  void startCollectSamples();
  void stopCalibration();
  void setCalibrationData(const std::vector<float> &serialData);

 private:
  using dispatcher = internal::CallbackDispatcher<CoreCallback>;

  int face_distance_mm = 600; // mm

  CoreCallback callback;

  /** DLL functions **/
  DLLFunction<const char*()> dGetVersion;

  struct SeeSo {};
  SeeSo* wrapper = nullptr;

  DLLFunction<SeeSo*(const char*, size_t, float, int32_t, int32_t, const int32_t*, int32_t)> dCreateSeeSo;
  DLLFunction<void(SeeSo*)> dDeleteSeeSo;

  DLLFunction<int(SeeSo*)> dDeinitEyeTracker;
  DLLFunction<int(SeeSo*, int)> dSetTrackingFps;
  DLLFunction<int(SeeSo*, float)> dSetCameraDistanceZ;
  DLLFunction<int(SeeSo*, float, float, float, float)> dSetCalibrationRegion;
  DLLFunction<int(SeeSo*, int, int)> dStartCalibration;
  DLLFunction<int(SeeSo*)> dStartCollectSamples;
  DLLFunction<int(SeeSo*)> dStopCalibration;
  DLLFunction<int(SeeSo*, const float* data, size_t size)> dSetCalibrationData;
  DLLFunction<int(SeeSo*, void *callback_obj,
                  internal::dispatch_c_t<decltype(&dispatcher::dispatchOnGaze)> on_gaze,
                  internal::dispatch_c_t<decltype(&dispatcher::dispatchOnStatus)> on_status,
                  internal::dispatch_c_t<decltype(&dispatcher::dispatchOnFace)> on_face,
                  internal::dispatch_c_t<decltype(&dispatcher::dispatchOnCalibrationNextPoint)> calib_next_point,
                  internal::dispatch_c_t<decltype(&dispatcher::dispatchOnCalibrationProgress)> calib_progress,
                  internal::dispatch_c_t<decltype(&dispatcher::dispatchOnCalibrationFinished)> calib_finish)> dSetCallbackInterface;
  DLLFunction<int(SeeSo*)> dRemoveCallbackInterface;
  DLLFunction<int(SeeSo*, int64_t, uint8_t*, int32_t, int32_t)> dAddFrame;
  DLLFunction<int(SeeSo*)> dGetAuthorizationResult;
};


} // namespace seeso


#endif //UNTITLED1_EYE_TRACKER_H
