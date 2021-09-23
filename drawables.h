//
// Created by YongGyu Lee on 2021/09/10.
//

#ifndef SEESO_SAMPLE_DRAWABLES_H_
#define SEESO_SAMPLE_DRAWABLES_H_

#include <algorithm>
#include <mutex>
#include <memory>
#include <string>
#include <type_traits>
#include <utility>

#include "opencv2/opencv.hpp"

/**
 * Drawable UI elements based on OpenCV drawings
 *
 * Non-static method `draw` does not check its visibility.
 * Use `draw_if` to draw with visibility checking
 */

namespace sample {
namespace drawables {
struct DrawableBase {
  bool visible = true;
};

struct Circle : protected DrawableBase {
  using DrawableBase::visible;

  void draw(cv::Mat* dst) const {
    cv::circle(*dst, center, radius, color, thickness, line_type, shift);
  }

  cv::Point center;
  int radius = 10;
  cv::Scalar color;
  int thickness = -1;
  int line_type = cv::LINE_8;
  int shift = 0;
};

struct Image : protected DrawableBase {
  using DrawableBase::visible;

  void draw(cv::Mat* dst) const {
    if (buffer.empty()) return;

    cv::resize(buffer, resized_, size);
    const auto img_w = std::min(resized_.cols, dst->cols - tl.x);
    const auto img_h = std::min(resized_.rows, dst->rows - tl.y);
    resized_(cv::Rect(0, 0, img_w, img_h)).copyTo((*dst)(cv::Rect(tl.x, tl.y, img_w, img_h)));
  }
  cv::Point tl;
  cv::Size size = {100, 100};
  cv::Mat buffer;

 private:
  mutable cv::Mat resized_;
};

struct Text : protected DrawableBase {
  using DrawableBase::visible;

  void draw(cv::Mat* dst) const {
    cv::putText(*dst, text, org, font_face, fontScale, color, thickness, line_type, bottom_left_origin);
  }

  cv::Point org;
  std::string text;
  int font_face = cv::FONT_HERSHEY_PLAIN;
  double fontScale = 1;
  cv::Scalar color = {255, 255, 255};
  int thickness = 1;
  int line_type = cv::LINE_8;
  bool bottom_left_origin = false;
};

template<typename...> using void_t = void;

template<typename T, typename = void>
struct is_drawable : std::false_type {};

template<typename T>
struct is_drawable<
  T,
  void_t<decltype(std::declval<typename std::add_const<T>::type>().draw(static_cast<cv::Mat*>(nullptr)))>
> : std::true_type {};

template<typename Drawable>
typename std::enable_if<is_drawable<Drawable>::value>::type
draw_if(const Drawable& drawable, cv::Mat* dst) {
  if (drawable.visible)
    drawable.draw(dst);
}

// No-own erasure
class Drawable {
  struct DrawableInterface {
    virtual void draw(cv::Mat* dst) const = 0;
  };

  template<typename T>
  struct DrawableConcrete : DrawableInterface {
    explicit DrawableConcrete(T* t) : object_(t) {}

    void draw(cv::Mat* dst) const override {
      if (object_->visible)
        object_->draw(dst);
    }

    T* object_;
  };

  std::shared_ptr<DrawableInterface> ptr_;

  template<typename T>
  T* get_ptr() {
    return dynamic_cast<DrawableConcrete<T>*>(ptr_.get())->object_;
  }

 public:
  Drawable() = default;

  template<typename T, typename std::enable_if<is_drawable<T>::value, int>::type = 0>
  explicit Drawable(T* object) : ptr_(std::make_shared<DrawableConcrete<T>>(object)) {}

  template<typename T> T* get_as() { return get_ptr<T>(); }
  template<typename T> const T* get_as() const { return get_ptr<T>(); }

  void draw(cv::Mat* dst) const {
    ptr_->draw(dst);
  }
};

} // namespace drawables
} // namespace sample

#endif // SEESO_SAMPLE_DRAWABLES_H_
