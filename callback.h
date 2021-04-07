//
// Created by cosge on 2021-03-25.
//

#ifndef SEESO_WINDOWS_CALLBACK_H
#define SEESO_WINDOWS_CALLBACK_H

#include <memory>
#include "seeso/callback_interface.h"
#include "seeso/util/display.h"
#include "seeso/util/coord_converter.h"
#include "view.h"

class Callback : public seeso::CallbackInterface {
 public:
  explicit Callback(const seeso::DisplayInfo &display);

  void OnGaze(uint64_t timestamp, float x, float y,
              seeso::TrackingState tracking_state, seeso::EyeMovementState eye_movement_state) override;
  void OnCalibrationProgress(float progress) override;
  void OnCalibrationNextPoint(float next_point_x, float next_point_y) override;
  void OnCalibrationFinished(std::vector<float> calib_data) override;
  void registerView(const std::shared_ptr<seeso::View> &view);

 private:
  std::shared_ptr<seeso::View> view;
  seeso::CoordConverter pc;
  seeso::WindowsDisplayUtil wdu;
};


#endif //SEESO_WINDOWS_CALLBACK_H
