//
// Created by cosge on 2021-04-27.
//

#ifndef SEESO_CALLBACK_IFACE_CALLBACK_H_
#define SEESO_CALLBACK_IFACE_CALLBACK_H_

namespace seeso {

/** experimental feature! not available yet */

class IFaceCallback {
 public:
  IFaceCallback() = default;
  virtual ~IFaceCallback() = default;

  IFaceCallback(const IFaceCallback&) = delete;
  IFaceCallback& operator = (const IFaceCallback&) = delete;

  virtual void OnLandmark() = 0;
  virtual void OnRegion() = 0;
};

}

#endif //SEESO_CALLBACK_IFACE_CALLBACK_H_
