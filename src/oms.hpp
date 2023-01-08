#pragma once

#include "common.hpp"
#include "throttler.hpp"

#include <cassert>
#include <fstream>
#include <memory>
#include <unordered_map>

// ASSUME: this class is created at the "right time" i.e. can trade as long as
// its alive
class OrderManagementSystem {
public:
  OrderManagementSystem() : m_latency_csv("latency.csv") { sendLogon(); }
  ~OrderManagementSystem() {
    // TODO: send cancels for all open orders??
    sendLogout();
  }
  OrderManagementSystem(const OrderManagementSystem &) = delete;
  OrderManagementSystem(OrderManagementSystem &&) = delete;
  OrderManagementSystem &operator=(const OrderManagementSystem &) = delete;
  OrderManagementSystem &operator=(OrderManagementSystem &&) = delete;

  void onData(OrderRequest &&request);

  void onData(OrderResponse &&response);

  void poll();

  template <IsOrderState T> bool isOrderState(Order::Id id) const {
    return std::holds_alternative<T>(m_open_orders.at(id)->m_state);
  }

private:
  // ASSUME: order_id is unique across the lifetime of this obj
  std::unordered_map<Order::Id, std::shared_ptr<Order>> m_open_orders;
  std::unordered_map<Order::Id, Clock::time_point> m_req_sent_at;
  std::ofstream m_latency_csv;

  std::deque<std::weak_ptr<Order>> m_queued_orders;
  Throttler m_throttler;

  void dequeue_orders();

  void send(Order &o, RequestType req);

  void send(const OrderRequest &) {}
  void sendLogon() {}
  void sendLogout() {}
};