#include "view.h"

namespace seeso {

View::View(int width, int height, std::string windowName)
:background(height, width, CV_8UC3, {0, 0, 0}), windowName(std::move(windowName)) {
  cv::namedWindow(this->windowName, cv::WINDOW_NORMAL);
}

void View::setPoint(long x, long y) {
  point.x = x;
  point.y = y;
}

void View::setFrame(cv::Mat frame) {
  this->frame = frame;
}

int View::draw() {
  background = cv::Scalar(0,0,0);
  frame.copyTo(background(cv::Rect(0, 0, std::min(frame.cols, background.cols - 0), std::min(frame.rows, background.rows - 0))));
  cv::circle(background, point, 10, color, -1, cv::LINE_AA);
  cv::imshow(windowName, background);
  int key = cv::waitKey(20);
  return key;
}

void View::closeWindow() {
  cv::destroyWindow(windowName);
}

std::string View::getWindowName() {
  return windowName;
}


}