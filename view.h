/**
 * Written by Dean on 2021-04-07.
 *
 * Create and delete windows, draw camera frames and gaze points.
 * */

#ifndef SEESO_SAMPLE_VIEW_H_
#define SEESO_SAMPLE_VIEW_H_

#include <string>
#include <vector>

#include "opencv2/opencv.hpp"

#include "drawables.h"

namespace sample {

class View {
 public:
  View(int width, int height, std::string windowName);

  void setPoint(int x, int y);
  void setFrame(const cv::Mat& frame);

  int draw(int wait_ms = 20);

  void closeWindow();

  const std::string& getWindowName() const;

  drawables::Circle gaze_point_;
  drawables::Circle calibration_point_;
  drawables::Text calibration_desc_;
  drawables::Image frame_;
  std::vector<drawables::Text> desc_;

 private:
  void initElements();

  void clearBackground();
  void drawElements();
  int drawWindow(int wait_ms);

  std::string window_name_;
  cv::Mat background_;
};

} // namespace sample

#endif // SEESO_SAMPLE_VIEW_H_
