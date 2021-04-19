#include "callback.h"
#include <iostream>

Callback::Callback(const seeso::DisplayInfo &display): pc(display), wdu() {}


void Callback::OnGaze(uint64_t timestamp, float x, float y, float fixation_x, float fixation_y,
                      seeso::TrackingState tracking_state, seeso::EyeMovementState eye_movement_state) {
  if (tracking_state == seeso::TrackingState::SUCCESS) {
    if (view != nullptr) {
      // Get the left top position of the 'seesosample' window
      auto winPos = wdu.getWindowPosition(view->getWindowName());
      // Convert the gaze xy point to a position on the screen.
      auto gazeOnScreenPos = pc.cameraToScreen(x, y);
      // Convert the gazeOnScreenPos to the value in the 'seesosample' window.
      auto gazePoint = pc.screenToWindow(gazeOnScreenPos, winPos);
      std::cout << "gazePoint: " << gazePoint.first << ", " << gazePoint.second << std::endl;
      // Pass gazePoint. gazePoint is element for drawing the seesosample window.
      view->setPoint(gazePoint.first, gazePoint.second);
    }
  } else {
    std::cout << "OnGaze not found: " << std::endl;
  }
}

void Callback::OnStatus(int32_t version, uint64_t timestamp, std::vector<float>& data) {
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
  std::cout << "Attention: " << attention << std::endl;
  std::cout << "Drowsiness: " << drowsiness << std::endl;
  std::cout << "Blink: " << blink << std::endl;
}

void Callback::OnFace(int32_t version, uint64_t timestamp, std::vector<float>& data) {
  // How should we display/use data?

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