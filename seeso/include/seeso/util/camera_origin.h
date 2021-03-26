#ifndef SEESO_UTIL_CAMERA_ORIGIN_H_
#define SEESO_UTIL_CAMERA_ORIGIN_H_

namespace seeso {

class CameraOrigin {
 public:
  explicit inline CameraOrigin(int cameraOriginXMm)
      : cameraOriginXMm(cameraOriginXMm) {}

  int inline getCameraOriginXMm() const {
    return cameraOriginXMm;
  }

 private:
  // mm
  int cameraOriginXMm;
};

} // namespace seeso

#endif // SEESO_UTIL_CAMERA_ORIGIN_H_