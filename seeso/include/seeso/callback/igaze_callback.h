//
// Created by cosge on 2021-04-27.
//

#ifndef SEESO_CALLBACK_IGAZE_CALLBACK_H_
#define SEESO_CALLBACK_IGAZE_CALLBACK_H_

#include <cstdint>

#include "seeso/framework/values.h"

namespace seeso {

class IGazeCallback {
 protected:
  IGazeCallback(const IGazeCallback&) = default;
  IGazeCallback& operator = (const IGazeCallback&) = default;

 public:
  IGazeCallback() = default;
  virtual ~IGazeCallback() = default;

  /** Gaze Callback
   *
   * @param timestamp           timestamp (passed by EyeTracker::AddFrame())
   * @param x                   mm in x-axis (camera coordinate)
   * @param y                   mm in y-axis (camera coordinate)
   * @param tracking_state      tracking state
   * @param eye_movement_state  eye movement state
   */
  virtual void OnGaze(uint64_t timestamp, float x, float y,
                      TrackingState tracking_state, EyeMovementState eye_movement_state) = 0;
};

} // namespace seeso

#endif //SEESO_CALLBACK_IGAZE_CALLBACK_H_
