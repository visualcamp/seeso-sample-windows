//
// Created by YongGyu Lee on 2021/06/20.
//

#include "priority_mutex.h"

namespace sample {

/** PriorityMutexHigh */
void PriorityMutex::HighMutex::lock() {
  return m_.lock_high();
}

void PriorityMutex::HighMutex::unlock() {
  return m_.unlock_high();
}

bool PriorityMutex::HighMutex::try_to_lock() {
  return m_.try_to_lock_high();
}

/** PriorityMutexLow */
void PriorityMutex::LowMutex::lock() {
  m_.lock_low();
}

void PriorityMutex::LowMutex::unlock() {
  m_.unlock_low();
}

bool PriorityMutex::LowMutex::try_to_lock() {
  return m_.try_to_lock_low();
}

/** PriorityMutex */
void PriorityMutex::lock_low() {
  std::unique_lock<mutex_type> lck(m_);
  cv_.wait(lck, [this]{
    return !high_accessing_;
  });
  lck.release();
}

void PriorityMutex::unlock_low() {
  m_.unlock();
  cv_.notify_one();
}

bool PriorityMutex::try_to_lock_low() {
  return !high_accessing_ && m_.try_lock();
}

void PriorityMutex::lock_high() {
  ++high_accessing_;
  m_.lock();
  --high_accessing_;
}

void PriorityMutex::unlock_high() {
  m_.unlock();
  cv_.notify_one();
}

bool PriorityMutex::try_to_lock_high() {
  ++high_accessing_;
  if (m_.try_lock()) {
    --high_accessing_;
    return true;
  }
  return false;
}

PriorityMutex::HighMutex& PriorityMutex::high() {
  return high_;
}

PriorityMutex::LowMutex& PriorityMutex::low() {
  return low_;
}

} // namespace sample
