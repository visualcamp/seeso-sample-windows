//
// Created by vcremote on 2021-04-06.
//

#include "figure.h"
//#include "opencv2/opencv.hpp"

namespace seeso{
    void Circle::drawOnImage(cv::Mat& image) {
        cv::circle(image, center, radius, color, thickness, line_type, shift);
    }
}