//
// Created by cosge on 2021-03-26.
//

#ifndef SEESO_VALUES_H_
#define SEESO_VALUES_H_

namespace seeso {

enum class TrackingState {
  SUCCESS = 0,
  LOW_CONFIDENCE,
  UNSUPPORTED,
  FACE_MISSING,
};

enum class EyeMovementState {
  FIXATION = 0,
  SACCADE = 2,
  UNKNOWN = 3,
};

enum class CalibrationAccuracy {
  DEFAULT = 0,
  LOW,
  HIGH,
};

enum TargetNum {
  ONE = 1,
  FIVE = 5,
};

}

#endif //SEESO_VALUES_H_
