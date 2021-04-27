#include "seeso/framework/core_callback.h"
#include <iostream>

namespace seeso {

namespace {

struct : public IGazeCallback {
  void OnGaze(uint64_t timestamp, float x, float y, TrackingState tracking_state,
              EyeMovementState eye_movement_state) override {}
} NullGazeCallback;

struct : public ICalibrationCallback {
  void OnCalibrationProgress(float progress) override {}
  void OnCalibrationNextPoint(float x, float y) override {}
  void OnCalibrationFinish(const std::vector<float> &data) override {}
} NullCalibrationCallback;

struct : public IStatusCallback {
  void OnAttention(uint64_t timestamp, float score) override {}
  void OnDrowsiness(uint64_t timestamp, bool isDrowsiness) override {}
  void OnBlink(uint64_t timestamp, bool isBlinkLeft, bool isBlinkRight, bool isBlink, float eyeOpenness) override {}
} NullStatusCallback;

struct : public IFaceCallback {
  void OnLandmark() override {}
  void OnRegion() override {}
} NullFaceCallback;

} // anonymous namespace


CoreCallback::CoreCallback()
: gaze_callback(&NullGazeCallback),
  calibration_callback(&NullCalibrationCallback),
  status_callback(&NullStatusCallback)
{}

void CoreCallback::OnGaze(uint64_t timestamp, float x, float y, float fixation_x, float fixation_y,
                          int movement_duration, seeso::TrackingState tracking_state, seeso::EyeMovementState eye_movement_state) {
  gaze_callback->OnGaze(timestamp, x, y, tracking_state, eye_movement_state);
}

void CoreCallback::OnStatus(uint64_t timestamp, std::vector<float> data) {
  float movement = data[0];
  float fixation = data[1];
  float EAR = data[2];
  float drowsiness = data[3];
  float ECR = data[4];
  float blink = data[5];
  float attention = data[6];
  float EARL = data[7];
  float EARR= data[8];
  float blinkR = data[9];
  float blinkL = data[10];

  if (isUseAttention) {
    status_callback->OnAttention(timestamp, attention);
  }

  if (isUseDrowsiness) {
    bool isDrowsiness = drowsiness >= 1.0f;
    status_callback->OnDrowsiness(timestamp, isDrowsiness);
  }

  if (isUseBlink) {
    bool isBlinkLeft = blinkL >= 1.0f;
    bool isBlinkRight = blinkR >= 1.0f;
    bool isBlink = blink >= 1.0f;
    status_callback->OnBlink(timestamp, isBlinkLeft, isBlinkRight, isBlink, EAR);
  }
}

void CoreCallback::OnFace(uint64_t timestamp, std::vector<float> data) {
}

void CoreCallback::OnCalibrationProgress(float progress) {
  calibration_callback->OnCalibrationProgress(progress);
}

void CoreCallback::OnCalibrationNextPoint(float next_point_x, float next_point_y) {
  calibration_callback->OnCalibrationNextPoint(next_point_x, next_point_y);
}

void CoreCallback::OnCalibrationFinished(std::vector<float> calib_data) {
  calibration_callback->OnCalibrationFinish(calib_data);
}

void CoreCallback::setStatusOptions(const std::vector<int>& statusOptions) {
  isUseAttention = false;
  isUseBlink = false;
  isUseDrowsiness = false;
  
  for (auto option : statusOptions) {
    if (option == seeso::StatusOptions::STATUS_ATTENTION) {
      isUseAttention = true;
    }
    else if (option == seeso::StatusOptions::STATUS_BLINK) {
      isUseBlink = true;
    }
    else if (option == seeso::StatusOptions::STATUS_DROWSINESS) {
      isUseDrowsiness = true;
    }
  }
}

void CoreCallback::setGazeCallback(seeso::IGazeCallback *callback) {
  gaze_callback = callback == nullptr ? &NullGazeCallback : callback;
}

void CoreCallback::setCalibrationCallback(seeso::ICalibrationCallback *callback) {
  calibration_callback = callback == nullptr ? &NullCalibrationCallback : callback;
}

void CoreCallback::setStatusCallback(seeso::IStatusCallback *callback) {
  status_callback = callback == nullptr ? &NullStatusCallback : callback;
}

}  // namespace seeso
