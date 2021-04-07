#ifndef SEESO_UTIL_POINT_CONVERTER_H_
#define SEESO_UTIL_POINT_CONVERTER_H_

#include <tuple>

#include "seeso/util/camera_origin.h"
#include "seeso/util/display.h"

namespace seeso {

class CoordConverter {
 public:
  explicit CoordConverter(const DisplayInfo &display_info);

  std::pair<long, long> cameraToScreen(float x, float y);
  std::pair<long, long> screenToWindow(std::pair<long, long> gazeOnScreenPos, std::pair<long, long> windowPos);

 private:
  float ppmX;
  float ppmY;
  seeso::DisplayInfo winDisplay;
  seeso::CameraOrigin camOrigin;
};

} // namespace seeso

#endif // SEESO_UTIL_POINT_CONVERTER_H_