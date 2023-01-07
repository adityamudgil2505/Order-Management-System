#include "order_management_system.hpp"

class Timer {
public:
  static long long get_time_in_ms() {
    std::chrono::time_point<std::chrono::system_clock> now =
        std::chrono::system_clock::now();
    auto duration = now.time_since_epoch();
    return std::chrono::duration_cast<std::chrono::milliseconds>(duration)
        .count();
  }
};
OrderManagementSystem::OrderManagementSystem(std::string username,
                                             std::string password)
    : username_{username}, password_{password} {
  this->max_processing_rate = 10;
  // this->min_time_ = (long)(10.0f / ((float) this->max_processing_rate));
  this->min_time_ = (long)(1000);
  this->last_action_ms = (long long)Timer::get_time_in_ms();
}
void OrderManagementSystem::onData(OrderRequest &&request) {
  // Dump the request
  std::cout << request.m_orderId << "\n";
  send(request);
}
void OrderManagementSystem::onData(OrderResponse &&response) {
  // update the request in db
}
void OrderManagementSystem::send(const OrderRequest &request) {}
void OrderManagementSystem::sendLogon() {}
void OrderManagementSystem::sendLogout() {}
void OrderManagementSystem::loadData() {
  long long current_time_ms = Timer::get_time_in_ms();
  long long time_left_ms;

  time_left_ms = this->last_action_ms + min_time_ - current_time_ms;
  if (time_left_ms > 0)
    this->last_action_ms += min_time_;
  else
    this->last_action_ms = current_time_ms;
  if (time_left_ms <= 0)
    return;
  else
    std::this_thread::sleep_for(std::chrono::milliseconds(time_left_ms));
  return;
}
void OrderManagementSystem::requestGateway() {
  auto start = Timer::get_time_in_ms();
  // Added condition so that it would not run forever
  while (Timer::get_time_in_ms() - start < 10000) {
    for (uint32_t i_cnt = 0; i_cnt < this->max_processing_rate; i_cnt++) {
      if (!this->order_queue_.is_empty()) {
        onData(this->order_queue_.pop());
      } else
        break;
    }
    loadData();
  }
}
OrderQueue &OrderManagementSystem::getOrderQueue() { return order_queue_; }