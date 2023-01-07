#ifndef ORDER_QUEUE_H
#define ORDER_QUEUE_H

#include "order_request.hpp"
#include <condition_variable>
#include <deque>
#include <iostream>
#include <mutex>
#include <unordered_map>

class OrderQueue {
private:
  std::deque<OrderRequest> q_;
  int size_;
  std::mutex mu_;
  std::condition_variable cond_;
  std::unordered_map<uint64_t, std::deque<OrderRequest>::iterator> hmap_;

public:
  OrderQueue();
  OrderRequest pop();
  void push(OrderRequest &order);
  void push(OrderRequest &&order);
  void update_order(OrderRequest &order);
  void remove_order(const uint64_t &order_id);
  bool is_empty();
  void handle_request(OrderRequest &order, const RequestType &type);
};

#endif