//
// Created by vcremote on 2021-04-06.
//

#ifndef SEESO_WINDOWS_FIGURE_H
#define SEESO_WINDOWS_FIGURE_H
#include "opencv2/opencv.hpp"
#include "Figure.h"

// D:\SeeSoSample-Windows\opencv\include\opencv2\opencv.hpp
namespace seeso {

class Figure {
public:
    Figure() = default;
    virtual ~Figure() = default;

    void setDefaultProps(const cv::Scalar& _color, int _thickness, int _line_type = cv::LINE_AA, int _shift = 0);
    template<typename T>
    void setDefaultProps(const std::initializer_list<T>& il, int _thickness, int _line_type = cv::LINE_AA, int _shift = 0);
    std::tuple<int, cv::Scalar, int, int> getProps() const;


    virtual void drawOnImage(cv::Mat& image) = 0;
protected:
    int thickness = 1;
    cv::Scalar color = {0, 0, 0};
    int line_type = cv::LINE_AA;
    int shift = 0;
};

class Circle : public Figure {
public:
    template<typename ...Args>
    Circle(const cv::Point& center, int radius, Args&&... props);
    virtual ~Circle() = default;
    void drawOnImage(cv::Mat& image) override;

protected:
    cv::Point center;
    int radius = 0;
};

}

#endif //SEESO_WINDOWS_FIGURE_H
