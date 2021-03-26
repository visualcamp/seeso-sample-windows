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
#include "seeso/values.h"

namespace seeso {


class EyeTracker {
 public:

  std::string getVersion() const;

  // load DLL library
  explicit EyeTracker(HINSTANCE procIDDLL);
  ~EyeTracker();

  // init SeeSo object
  int initialize(const std::string& license_key);
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

  int face_distance_mm = 600; // mm

  /** DLL functions **/
  DFunction<const char*()> dGetVersion;

  struct SeeSo {};
  SeeSo* wrapper = nullptr;

  DFunction<SeeSo*(const char*, size_t, float, int32_t, int32_t)> dCreateSeeSo;
  DFunction<void(SeeSo*)> dDeleteSeeSo;

  DFunction<int(SeeSo*)> dDeinitEyeTracker;
  DFunction<int(SeeSo*, int)> dSetTrackingFps;
  DFunction<int(SeeSo*, float)> dSetCameraDistanceZ;
  DFunction<int(SeeSo*, float, float, float, float)> dSetCalibrationRegion;
  DFunction<int(SeeSo*, int, int)> dStartCalibration;
  DFunction<int(SeeSo*)> dStartCollectSamples;
  DFunction<int(SeeSo*)> dStopCalibration;
  DFunction<int(SeeSo*, const float* data, size_t size)> dSetCalibrationData;
  DFunction<int(SeeSo*, void *callback_obj,
                void(*on_gaze           )(void*, uint64_t, float, float, int, int),
                void(*calib_next_point  )(void*, float, float),
                void(*calib_progress    )(void*, float),
                void(*calib_finish      )(void*, float*, int))> dSetCallbackInterface;
  DFunction<int(SeeSo*)> dRemoveCallbackInterface;
  DFunction<int(SeeSo*, int64_t, uint8_t*, int, int)> dAddFrame;
  DFunction<int(SeeSo*)> dGetAuthorizationResult;
};


} // namespace seeso


#endif //UNTITLED1_EYE_TRACKER_H
