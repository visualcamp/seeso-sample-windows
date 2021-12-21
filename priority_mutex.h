//
// Created by YongGyu Lee on 2021/06/20.
//

#ifndef SEESO_SAMPLE_PRIORITY_MUTEX_H_
#define SEESO_SAMPLE_PRIORITY_MUTEX_H_

#include <atomic>
#include <condition_variable>
#include <mutex>

namespace sample {

class PriorityMutex {
  class HighMutex {
   public:
    explicit HighMutex(PriorityMutex& m) noexcept : m_(m) {}

    void lock();
    void unlock();
    bool try_to_lock();

    HighMutex(HighMutex const&) = delete;
    HighMutex(HighMutex &&) = delete;
    HighMutex& operator=(HighMutex const&) = delete;
    HighMutex& operator=(HighMutex &&) = delete;

   private:
    PriorityMutex& m_;
  };

  class LowMutex {
   public:
    explicit LowMutex(PriorityMutex& m) noexcept : m_(m) {}

    void lock();
    void unlock();
    bool try_to_lock();

    LowMutex(LowMutex const&) = delete;
    LowMutex(LowMutex &&) = delete;
    LowMutex& operator=(LowMutex const&) = delete;
    LowMutex& operator=(LowMutex &&) = delete;

   private:
    PriorityMutex& m_;
  };

 public:
  using mutex_type = std::mutex;
  using low_mutex_type = LowMutex;
  using high_mutex_type = HighMutex;

  void lock_low();
  void unlock_low();
  bool try_to_lock_low();
  LowMutex& low();

  void lock_high();
  void unlock_high();
  bool try_to_lock_high();
  HighMutex& high();

 private:
  mutex_type m_;
  std::condition_variable cv_;
  std::atomic_int high_accessing_{0};
  low_mutex_type low_{*this};
  high_mutex_type high_{*this};
};

} // namespace sample

#endif // SEESO_SAMPLE_PRIORITY_MUTEX_H_
