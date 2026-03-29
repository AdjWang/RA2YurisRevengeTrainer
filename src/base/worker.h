#pragma once
#include "boost/asio.hpp"  // include before WinSock.h

namespace yrtr {

class Worker {
 public:
  Worker(std::string_view name)
      : name_(name), work_guard_(boost::asio::make_work_guard(io_context_)) {
    thread_ = std::thread([this]() { io_context_.run(); });
  }

  ~Worker() {
    work_guard_.reset();
    io_context_.stop();
    if (thread_.joinable()) {
      thread_.join();
    }
    DLOG_F(INFO, "Worker name={} exit", name_);
  }

  // Thread-safe.
  void PostTask(std::function<void()> task) {
    boost::asio::post(io_context_, std::move(task));
  }

 private:
  const std::string name_;
  boost::asio::io_context io_context_;
  boost::asio::executor_work_guard<boost::asio::io_context::executor_type>
      work_guard_;
  std::thread thread_;
};

}  // namespace yrtr
