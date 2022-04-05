#include "view.h"

#include <cstring> // for memset

#include <algorithm>
#include <utility>

namespace sample {

View::View(int width, int height, std::string windowName)
: background_(height, width, CV_8UC3, {0, 0, 0}), window_name_(std::move(windowName)) {
  cv::namedWindow(window_name_);
  initElements();
}

void View::setPoint(int x, int y) {
  gaze_point_.center = {x, y};
}

void View::setFrame(const cv::Mat& frame) {
  frame_.buffer = frame;
}

int View::draw(int wait_ms) {
  clearBackground();
  drawElements();
  return drawWindow(wait_ms);
}

void View::closeWindow() {
  cv::destroyWindow(window_name_);
}

const std::string& View::getWindowName() const {
  return window_name_;
}

void View::initElements() {
  gaze_point_.color = {0, 220, 220};

  calibration_point_.visible = false;
  calibration_point_.color = {0, 0, 255};
  calibration_point_.radius = 50;

  calibration_desc_.text = "Stare at the red circle until it disappears or moves to other place.";
  calibration_desc_.org = {background_.cols / 2, background_.rows / 2};
  calibration_desc_.visible = false;

  frame_.size = {480, 320};

  desc_.resize(2);
  desc_[0].text = "Press ESC to exit program, Press 'C' to start calibration";
  desc_[1].text = "Do not resize the window manually after created";
  desc_[1].color = {0, 0, 220};

  for (int i = 0; i < desc_.size(); ++i) {
    desc_[desc_.size() - 1 - i].fontScale = 1.5;
    desc_[desc_.size() - 1 - i].org = {50, background_.rows - 50 * (i + 1)};
  }
}

void View::clearBackground() {
  std::memset(background_.data, 0, background_.rows * background_.cols * background_.channels());
}

void View::drawElements() {
  read_lock_guard lock(read_mutex());

  drawables::draw_if(frame_, &background_);
  drawables::draw_if(gaze_point_, &background_);
  drawables::draw_if(calibration_point_, &background_);
  drawables::draw_if(calibration_desc_, &background_);
  for (const auto& desc : desc_)
    drawables::draw_if(desc, &background_);
}

int View::drawWindow(int wait_ms) {
  cv::imshow(window_name_, background_);
  int key = cv::waitKey(wait_ms);
  return key;
}

} // namespace sample
