//
// Created by vcremote on 2021-04-07.
//

#include "view.h"

namespace seeso {

View::View(int width, int height, std::string windowName)
:background(height, width, CV_8UC3, {0, 0, 0}), windowName(std::move(windowName)) {
  cv::namedWindow(windowName);
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
  frame.copyTo(background(cv::Rect(640, 480, std::min(frame.cols, std::max(0, background.cols - 640)), std::min(frame.rows, std::max(0, background.rows - 480)))));
  cv::circle(background, point, 10, color, -1, cv::LINE_AA);
  cv::imshow(windowName, background);
  int key = cv::waitKey(20);
  return key;
}

void View::closeWindow() {
  cv::destroyWindow(windowName);
}


}