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
#include "priority_mutex.h"

namespace sample {

using read_lock_guard = std::lock_guard<typename PriorityMutex::high_mutex_type>;
using read_unique_lock = std::unique_lock<typename PriorityMutex::high_mutex_type>;

using write_lock_guard = std::lock_guard<typename PriorityMutex::low_mutex_type>;
using write_unique_lock = std::unique_lock<typename PriorityMutex::low_mutex_type>;

class View {
 public:
  View(int width, int height, std::string windowName);

  void setPoint(int x, int y);
  void setFrame(const cv::Mat& frame);

  int draw(int wait_ms = 10);

  void closeWindow();

  const std::string& getWindowName() const;

  drawables::Circle gaze_point_;
  drawables::Circle calibration_point_;
  drawables::Text calibration_desc_;
  drawables::Image frame_;
  std::vector<drawables::Text> desc_;

  PriorityMutex::low_mutex_type& write_mutex() { return mutex_.low(); }

 private:
  void initElements();

  void clearBackground();
  void drawElements();
  int drawWindow(int wait_ms);

  PriorityMutex::high_mutex_type& read_mutex() { return mutex_.high(); }

  std::string window_name_;
  cv::Mat background_;
  mutable PriorityMutex mutex_;
};

} // namespace sample

#endif // SEESO_SAMPLE_VIEW_H_
