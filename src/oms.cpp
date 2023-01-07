#include "oms.hpp"

void OrderManagementSystem::onData(OrderRequest &&request) {
  // ASSUME: modify requests are well formed and will be accepted by
  // exchange
  assert(request.m_req_type != RequestType::Unknown);

  auto [it, inserted] = m_open_orders.try_emplace(
      request.m_orderId,
      std::make_shared<Order>(OrderState::Resting{}, request.m_side,
                              request.m_symbolId, request.m_price,
                              request.m_qty, request.m_orderId));
  if (std::holds_alternative<OrderState::RequestPending>(it->second->m_state)) {
    throw std::logic_error("Request is pending");
  } else if (request.m_req_type == RequestType::Cancel) {

    m_req_sent_at[request.m_orderId] = Clock::now();
    std::erase_if(m_queued_orders,
                  [id = request.m_orderId](const std::weak_ptr<Order> &o) {
                    return o.expired() || o.lock()->m_id == id;
                  });
    send(*it->second, RequestType::Cancel);
    m_open_orders.erase(request.m_orderId);

  } else {

    // order state can be resting / queued
    //
    // order can be sent straightaway or can be queued. these two cases can be
    // handled uniformly by first queueing the order and if throttling allows
    // then the order can be dequeued right away
    Order &o = *it->second;
    o.m_symbolId = request.m_symbolId;
    o.m_price = request.m_price;
    o.m_qty = request.m_qty;
    o.m_side = request.m_side;
    if (!o.isQueued()) {
      m_queued_orders.push_back(it->second);
    }
    o.m_state = OrderState::Queued{Clock::now(), request.m_req_type};
    dequeue_orders();
  }
}

void OrderManagementSystem::onData(OrderResponse &&response) {
  assert(response.m_responseType == ResponseType::Accept);

  if (auto it = m_open_orders.find(response.m_orderId);
      it != m_open_orders.end()) {
    assert(std::holds_alternative<OrderState::RequestPending>(
        it->second->m_state));
    it->second->m_state = OrderState::Resting{};
  }

  // persist latency
  const auto sent_at = m_req_sent_at.at(response.m_orderId);
  m_req_sent_at.erase(response.m_orderId);
  m_latency_csv << response.m_orderId << ","
                << sent_at.time_since_epoch().count() << ","
                << Clock::now().time_since_epoch().count() << "\n";
}

void OrderManagementSystem::poll() { dequeue_orders(); }

void OrderManagementSystem::dequeue_orders() {
  while (m_throttler.canSend() && !m_queued_orders.empty()) {
    std::shared_ptr o = m_queued_orders.front().lock();
    m_queued_orders.pop_front();
    if (o) {
      send(*o, std::get<OrderState::Queued>(o->m_state).m_type);
      m_throttler.notify_req_sent(Clock::now());
    }
  }
}

void OrderManagementSystem::send(Order &o, RequestType req) {
  assert(!m_req_sent_at.contains(o.m_id));
  m_req_sent_at[o.m_id] = Clock::now();
  o.m_state = OrderState::RequestPending{};
  send(OrderRequest{o.m_symbolId, o.m_price, o.m_qty, o.m_side, o.m_id, req});
}
