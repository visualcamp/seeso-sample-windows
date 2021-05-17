#include "callback.h"

#include "seeso/util/display.h"

#include <iostream>

Callback::Callback(const seeso::DisplayInfo &display) : pc(display) {}


void Callback::OnGaze(uint64_t timestamp,
                      float x, float y,
                      seeso::TrackingState tracking_state,
                      seeso::EyeMovementState eye_movement_state) {
  if (view == nullptr)
    return;

  if (tracking_state == seeso::TrackingState::SUCCESS) {

    // Convert the gaze point to a position on the display monitor
    // Get the top-left coordinate of the 'seesosample' window
    // Convert the display point to the position in the 'seesosample' window
    // Pass gazePoint. gazePoint is element for drawing the seesosample window.

    auto gazeOnScreenPos = pc.cameraToScreen<float>({x, y});
    auto winPos = seeso::getWindowPosition(view->getWindowName());
    auto gazePoint = pc.screenToWindow<float>(gazeOnScreenPos, winPos);

    std::cout << "gazePoint: " << gazePoint.x << ", " << gazePoint.y << std::endl;
    view->setPoint(gazePoint.x, gazePoint.y);
  } else {
    std::cout << "OnGaze not found: " << std::endl;
  }
}

void Callback::OnAttention(uint64_t timestampBegin, uint64_t timestampEnd, float score) {
  std::cout << "Attention: " << score << std::endl;
}

void Callback::OnBlink(uint64_t timestamp, bool isBlinkLeft, bool isBlinkRight, bool isBlink, float eyeOpenness) {
  std::cout << "Blink: " << isBlink << ", " << isBlinkLeft << ", " << isBlinkRight << std::endl;
  std::cout << "EyeOpenness: " << eyeOpenness << std::endl;
}

void Callback::OnDrowsiness(uint64_t timestamp, bool isDrowsiness) {
  std::cout << "Drowsiness: " << isDrowsiness << std::endl;
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

void Callback::registerView(std::shared_ptr<seeso::View> view) {
  this->view = std::move(view);
}