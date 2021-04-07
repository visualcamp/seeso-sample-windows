
#include "seeso/util/coord_converter.h"
#include <tuple>

namespace seeso {

CoordConverter::CoordConverter(const DisplayInfo &display_info)
    : camOrigin(display_info.widthMm / 2),
      winDisplay(display_info)
{
  ppmX = (float) winDisplay.widthPx / (float) winDisplay.widthMm;
  ppmY = (float) winDisplay.heightPx / (float) winDisplay.heightMm;
}

std::pair<long, long> CoordConverter::cameraToScreen(float x, float y) {
  x = ppmX * (x + camOrigin.getCameraOriginXMm());
  y = ppmY * -(y);
  return {(long)x, (long)y};
}

std::pair<long, long> CoordConverter::screenToWindow(std::pair<long, long> gazeOnScreenPos, std::pair<long, long> windowPos) {
    long gazeXInWindow = gazeOnScreenPos.first - windowPos.first;
    long gazeYInWindow = gazeOnScreenPos.second - windowPos.second;
    return { gazeXInWindow, gazeYInWindow };
}


} // namespace seeso
