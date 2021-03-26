//
// Created by cosge on 2021-03-25.
//

#include "callback.h"
#include <iostream>

Callback::Callback(const seeso::DisplayInfo &display): pc(display) {}

void Callback::OnGaze(uint64_t timestamp, float x, float y,
                      seeso::TrackingState tracking_state, seeso::EyeMovementState eye_movement_state) {
  if (tracking_state == seeso::TrackingState::SUCCESS) {
    auto result = pc.cameraToScreen(x, y);
    std::cout << "OnGaze cam: " << x << " x " << y << ", screen: " << result.first << " x " << result.second << std::endl;
  } else {
    std::cout << "OnGaze not found: " << std::endl;
  }

}

void Callback::OnCalibrationProgress(float progress) {

}

void Callback::OnCalibrationNextPoint(float next_point_x, float next_point_y) {

}

void Callback::OnCalibrationFinished(std::vector<float> calib_data) {

}