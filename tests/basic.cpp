#include "oms.hpp"

int main() {
  OrderManagementSystem oms;
  oms.onData(OrderRequest{1, 1e4, 20, 'B', 1, RequestType::New});
  assert(oms.isOrderState<OrderState::RequestPending>(1));

  // send response
  oms.onData(OrderResponse{1, ResponseType::Accept});
  assert(oms.isOrderState<OrderState::Resting>(1));

  // spam orders
  for (Order::Id id = 2; id <= 501; ++id) {
    oms.onData(OrderRequest{1, 1e4, 10, 'B', id, RequestType::New});
  }
  // dequeue some orders
  oms.poll();

  oms.onData(OrderRequest{1, 1e4, 30, 'B', 1, RequestType::Modify});
  assert(oms.isOrderState<OrderState::Queued>(1));

  oms.onData(OrderRequest{1, 1e4, 40, 'B', 1, RequestType::Modify});
  assert(oms.isOrderState<OrderState::Queued>(1));
}