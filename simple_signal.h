/**
 * Created by YongGyu Lee on 2021/09/16.
 *
 * Simple imitation of boost::signal without grouping, etc
 */

#ifndef SEESO_SAMPLE_SIMPLE_SIGNAL_H_
#define SEESO_SAMPLE_SIMPLE_SIGNAL_H_

#include <list>
#include <memory>
#include <mutex>
#include <utility>

namespace sample {

template<typename F>
class signal;

class slot_base {
 public:
  ~slot_base() = default;
  virtual void expire() = 0;
};

template<typename R, typename ...Args>
class slot : public slot_base {
 public:
  template<typename F2>
  explicit slot(const F2& func) : func_(func) {}

  template<typename ...Ts>
  R operator()(Ts&&... args) const {
    return func_(std::forward<Ts>(args)...);
  }

  void expire() override {
    expired_ = true;
  }

  bool expired() const {
    return expired_;
  }

 private:
  std::atomic_bool expired_{false};
  std::function<R(Args...)> func_;
};

/**
 * Stores signal's connection and can disconnect.
 * Does not automatically disconnect when destructed, copied or moved.
 */
class connection {
 public:
  connection() = default;

  void disconnect() {
    auto lock_ptr = slot_ptr_.lock();
    if (lock_ptr) {
      lock_ptr->expire();
    }
  }

 private:
  template<typename F> friend class signal;

  template<typename ...T>
  explicit connection(std::shared_ptr<slot<T...>> ptr) : slot_ptr_(std::move(ptr)) {}

  template<typename ...T>
  connection& operator=(std::shared_ptr<slot<T...>> ptr) {
    slot_ptr_ = std::move(ptr);
    return *this;
  }

  std::weak_ptr<slot_base> slot_ptr_;
};

/**
 * Stores signal's connection and can disconnect.
 * Automatically disconnects stored connection when destructed or moved.
 */
class raii_connection {
 public:
  raii_connection() = default;
  ~raii_connection() { conn_.disconnect(); }

  raii_connection(const raii_connection&) = delete;
  raii_connection& operator=(const raii_connection&) = delete;

  raii_connection(raii_connection&&) = default;
  raii_connection& operator=(raii_connection&&) = default;

  explicit raii_connection(connection&& conn) : conn_(std::move(conn)) {}
  raii_connection& operator=(connection&& conn) {
    conn_.disconnect();
    conn_ = std::move(conn);
    return *this;
  }
 private:
  connection conn_;
};

/**
 * Imitates boost::signal
 * Can connect(register) functions and notify connected functions.
 * One must store connection from signal::connect in order to disconnect(unregister) it later.
 *
 * @tparam R        return type
 * @tparam Args     argument type
 */
template<typename R, typename ...Args>
class signal<R(Args...)> {
 public:
  using function_type = std::function<R(Args...)>;
  using slot_type = slot<R, Args...>;
  using slot_list = std::list<std::shared_ptr<slot_type>>;

  /**
   * Connect a function
   * @param func
   * @return connection
   */
  connection connect(function_type func) {
    auto new_slot = std::make_shared<slot_type>(std::move(func));
    {
      std::lock_guard<std::mutex> lck(connect_mutex_);
      slot_list_.emplace_back(new_slot);
    }

    return connection(new_slot);
  }

  /**
   * Connect a function and a tracking target.
   * A function is not invoked if the tracking target's life has ended
   *
   * @tparam T
   * @param func
   * @param track
   * @return connection
   */
  template<typename T>
  connection connect(function_type func, std::shared_ptr<T> track) {
    std::weak_ptr<void> weak_ptr = std::move(track);
    return connect([=](Args&&... args) {
      auto lck = weak_ptr.lock();
      if (lck) {
        func(std::forward<Args>(args)...);
      }
    });
  }

  /**
   * Invokes connected functions.
   * @tparam Args2
   * @param args
   */
  template<typename ...Args2>
  void operator()(Args2&&... args) {
    std::unique_lock<std::mutex> lck(connect_mutex_);
    auto it = slot_list_.begin();

    while (it != slot_list_.end()) {
      if ((*it)->expired()) {
        it = slot_list_.erase(it);
        continue;
      }

      lck.unlock();
      (**it)(args...);
      ++it;
      lck.lock();
    }
  }

 private:
  mutable std::mutex connect_mutex_;
  slot_list slot_list_;
};

} // namespace sample

#endif // SEESO_SAMPLE_SIMPLE_SIGNAL_H_
