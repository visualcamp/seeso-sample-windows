//
// Created by YongGyu Lee on 2021/09/16.
//

#ifndef SEESO_SAMPLE_CAMERA_THREAD_H_
#define SEESO_SAMPLE_CAMERA_THREAD_H_

#include <atomic>
#include <thread>
#include <mutex>
#include <condition_variable>

#include "opencv2/opencv.hpp"

#include "simple_signal.h"

namespace sample {

/**
 * A handler that runs camera in concurrent thread.
 * Use on_frame_ to add or remove frame listeners
 */
class CameraThread {
 public:
  CameraThread();
  ~CameraThread();

  bool run(int camera_index = 0);

  void resume();
  void pause();

  void join();

  signal<void(cv::Mat frame)> on_frame_;

 private:
  void run_impl();
  bool check_status();
  std::unique_lock<std::mutex> pause_wait();

  int camera_index_ = 0;
  cv::VideoCapture video_;
  cv::Mat frame_;

  std::thread thread_;
  std::atomic_bool pause_{true};
  std::mutex mutex_;
  std::condition_variable cv_;

  std::atomic_bool stop_{false};
};

} // namespace sample

#endif // SEESO_SAMPLE_CAMERA_THREAD_H_
