#include "callback.h"
#include <iostream>

Callback::Callback(const seeso::DisplayInfo &display): pc(display), wdu() {}


void Callback::OnGaze(uint64_t timestamp, float x, float y,
                      seeso::TrackingState tracking_state, seeso::EyeMovementState eye_movement_state) {
  if (tracking_state == seeso::TrackingState::SUCCESS) {
    if (view != nullptr) {
      // Get the left top position of the 'seesosample' window
      auto winPos = wdu.getWindowPosition(view->getWindowName());
      // Convert the gaze xy point to a position on the screen.
      auto gazeOnScreenPos = pc.cameraToScreen<float>(x, y);
      // Convert the gazeOnScreenPos to the value in the 'seesosample' window.
      auto gazePoint = pc.screenToWindow<float>(gazeOnScreenPos, winPos);
      //std::cout << "gazePoint: " << gazePoint.first << ", " << gazePoint.second << std::endl;
      // Pass gazePoint. gazePoint is element for drawing the seesosample window.
      view->setPoint(gazePoint.first, gazePoint.second);
    }
  } else {
   // std::cout << "OnGaze not found: " << std::endl;
  }
}

void Callback::OnAttention(uint64_t timestamp, float score) {
  std::cout << "Attention: " << score << std::endl;
}

void Callback::OnBlink(uint64_t timestamp, bool isBlinkLeft, bool isBlinkRight, bool isBlink, float eyeOpenness) {
 // std::cout << "Blink: " << isBlink << ", " << isBlinkLeft << ", " << isBlinkRight << std::endl;
  //std::cout << "EyeOpenness: " << eyeOpenness << std::endl;
}

void Callback::OnDrowsiness(uint64_t timestamp, bool isDrowsiness) {
 // std::cout << "Drowsiness: " << isDrowsiness << std::endl;
}

void Callback::OnCalibrationProgress(float progress) {
  // Not yet implemented.
}

void Callback::OnCalibrationNextPoint(float next_point_x, float next_point_y) {
  // Not yet implemented.
}

void Callback::OnCalibrationFinish(const std::vector<float>& calib_data) {
  // Not yet implemented.
}

void Callback::registerView(const std::shared_ptr<seeso::View> &view) {
  this->view = view;
}