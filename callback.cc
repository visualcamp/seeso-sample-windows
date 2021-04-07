/**
 * Written by Dean on 2021-04-07.
 * 
 * The function of the Callback class is registered in the'EyeTracker' class.
 * When EyeTracker class initialized normally, each function of 'Callback' class is called in time.
 * */

#include "callback.h"
#include <iostream>

Callback::Callback(const seeso::DisplayInfo &display): pc(display), wdu() {}

void Callback::OnGaze(uint64_t timestamp, float x, float y,
                      seeso::TrackingState tracking_state, seeso::EyeMovementState eye_movement_state) {
  if (tracking_state == seeso::TrackingState::SUCCESS) {
    auto winPos = wdu.getWindowPosition();
    auto gazeOnScreenPos = pc.cameraToScreen(x, y);
    auto gazePoint = pc.screenToWindow(gazeOnScreenPos, winPos);
    std::cout << "gazePoint: " << gazePoint.first << ", " << gazePoint.second << std::endl;
    if (view != nullptr) view->setPoint(gazePoint.first, gazePoint.second);
  } else {
    std::cout << "OnGaze not found: " << std::endl;
  }
}

void Callback::OnCalibrationProgress(float progress) {
  // Not yet implemented.
}

void Callback::OnCalibrationNextPoint(float next_point_x, float next_point_y) {
  // Not yet implemented.
}

void Callback::OnCalibrationFinished(std::vector<float> calib_data) {
  // Not yet implemented.
}

void Callback::registerView(const std::shared_ptr<seeso::View> &view) {
  this->view = view;
}