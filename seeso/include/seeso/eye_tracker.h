//
// Created by cosge on 2021-03-24.
//
#pragma once

#include <vector>
#include <string>

#include "seeso/framework/core_callback.h"
#include "seeso/framework/values.h"
#include "seeso/framework/user_status_option.h"
#include "seeso/callback/icalibration_callback.h"
#include "seeso/callback/igaze_callback.h"
#include "seeso/callback/istatus_callback.h"

namespace seeso {

// This function must be called before any function or EyeTracker object is created.
void global_init(const char* file = "seeso_core.dll");

std::string getVersionStr();

int32_t getVersionInt();

class EyeTracker {
 public:

  ~EyeTracker();

  // init SeeSo object
  int initialize(const std::string& license_key, const UserStatusOption& options);
  // destroy SeeSo object
  void deinitialize();

  // set the distance between face and camera
  void setFaceDistance(int cm);
  int getFaceDistance() const; // return cm

  // set listener
  void setGazeCallback(seeso::IGazeCallback* listener);
  void setCalibrationCallback(seeso::ICalibrationCallback* listener);
  void setStatusCallback(seeso::IStatusCallback* listener);


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

  // set target region bound
  void setTargetBoundRegion(float left, float top, float right, float bottom);

  // Calibration
  void startCalibration(TargetNum num, CalibrationAccuracy criteria,
                         float left, float top, float right, float bottom);
//  void setCalibrationRegion(float left, float top, float right, float bottom);
//  void startCalibration(int mode, int criteria);
  void startCollectSamples();
  void stopCalibration();
  void setCalibrationData(const std::vector<float> &serialData);

 private:
  CoreCallback callback;
  int face_distance_mm = 600; // mm

  void* seeso_object = nullptr;
};


} // namespace seeso
