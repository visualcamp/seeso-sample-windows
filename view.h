//
// Created by vcremote on 2021-04-07.
//

#ifndef SEESO_WINDOWS_VIEW_H
#define SEESO_WINDOWS_VIEW_H

#include "opencv2/opencv.hpp"
#include <string>

namespace seeso {
class View {
public:
  View(int width, int height, std::string windowName);
  void setPoint(long x, long y);
  void setFrame(cv::Mat frame);
  int draw();
  void closeWindow();
  std::string getWindowName();
private:
  std::string windowName;
  cv::Point point;
  cv::Mat background;
  cv::Mat frame;
  cv::Scalar color{3, 127, 252};
};

}
#endif //SEESO_WINDOWS_VIEW_H
