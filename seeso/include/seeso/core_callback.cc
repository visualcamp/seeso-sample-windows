#include "core_callback.h"
#include <iostream>

namespace seeso {

void CoreCallback::OnGaze(uint64_t timestamp, float x, float y, float fixation_x, float fixation_y,
                          int movement_duration, seeso::TrackingState tracking_state, seeso::EyeMovementState eye_movement_state) {
  if (callback != nullptr) {
    callback->OnGaze(timestamp, x, y, tracking_state, eye_movement_state);
  }
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

  if (callback != nullptr) {
    if (isUseAttention) {
      callback->OnAttention(timestamp, attention);
    }

    if (isUseDrowsiness) {
      bool isDrowsiness = drowsiness >= 1.0f;
      callback->OnDrowsiness(timestamp, isDrowsiness);
    }

    if (isUseBlink) {
      bool isBlinkLeft = blinkL >= 1.0f;
      bool isBlinkRight = blinkR >= 1.0f;
      bool isBlink = blink >= 1.0f;
      callback->OnBlink(timestamp, isBlinkLeft, isBlinkRight, isBlink, EAR);
    } 
  }
}

void CoreCallback::OnFace(uint64_t timestamp, std::vector<float> data) {
  // How should we display/use data?
  if (callback != nullptr) {
    // Not yet implemented
  }
}

void CoreCallback::OnCalibrationProgress(float progress) {
  // Not yet implemented.
  if (callback != nullptr) {
    callback->OnCalibrationProgress(progress);
  }
}

void CoreCallback::OnCalibrationNextPoint(float next_point_x, float next_point_y) {
  // Not yet implemented.
  if (callback != nullptr) {
    callback->OnCalibrationNextPoint(next_point_x, next_point_y);
  }
}

void CoreCallback::OnCalibrationFinished(std::vector<float> calib_data) {
  // Not yet implemented.
  if (callback != nullptr) {
    callback->OnCalibrationFinished(calib_data);
  }
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

void CoreCallback::setCallbackInterface(seeso::CallbackInterface* callback_obj) {
  callback = callback_obj;
}

void CoreCallback::removeCallbackInterface() {
  callback = nullptr;
}

}  // namespace seeso
