#include "core_callback.h"
#include <iostream>

seeso::CoreCallback::CoreCallback() {
 
}

void seeso::CoreCallback::OnGaze(uint64_t timestamp, float x, float y, float fixation_x, float fixation_y,
                      seeso::TrackingState tracking_state, seeso::EyeMovementState eye_movement_state) {
  if (callback != nullptr) {
    callback->OnGaze(timestamp, x, y, tracking_state, eye_movement_state);
  }
}

void seeso::CoreCallback::OnStatus(uint64_t timestamp, std::vector<float> data) {
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
      callback->OnDrowsiness(timestamp, drowsiness);
    }

    if (isUseBlink) {
      callback->OnBlink(timestamp, blinkL, blinkR, blink, EAR);
    } 
  }
}

void seeso::CoreCallback::OnFace(uint64_t timestamp, std::vector<float> data) {
  // How should we display/use data?
  if (callback != nullptr) {
    // Not yet implemented
  }
}

void seeso::CoreCallback::OnCalibrationProgress(float progress) {
  // Not yet implemented.
  if (callback != nullptr) {
    callback->OnCalibrationProgress(progress);
  }
}

void seeso::CoreCallback::OnCalibrationNextPoint(float next_point_x, float next_point_y) {
  // Not yet implemented.
  if (callback != nullptr) {
    callback->OnCalibrationNextPoint(next_point_x, next_point_y);
  }
}

void seeso::CoreCallback::OnCalibrationFinished(std::vector<float> calib_data) {
  // Not yet implemented.
  if (callback != nullptr) {
    callback->OnCalibrationFinished(calib_data);
  }
}

void seeso::CoreCallback::setStatusOptions(const std::vector<int>& statusOptions) {
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

void seeso::CoreCallback::setCallbackInterface(seeso::CallbackInterface* callback_obj) {
  callback = callback_obj;
}

void seeso::CoreCallback::removeCallbackInterface() {
  callback = nullptr;
}
