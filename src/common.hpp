#pragma once

#include <chrono>
#include <cstdint>
#include <string>
#include <variant>

struct Logon {
  std::string username;
  std::string password;
};

struct Logout {
  std::string username;
};

/////////////////////// Request / Reponse ///////////////////////////
enum class RequestType { Unknown = 0, New = 1, Modify = 2, Cancel = 3 };

struct OrderRequest {
  int m_symbolId;
  double m_price;
  uint64_t m_qty;
  char m_side; // possible values 'B' or 'S'
  uint64_t m_orderId;
  RequestType m_req_type; // TODO: add this to assumption
};

enum class ResponseType {
  Unknown = 0,
  Accept = 1,
  Reject = 2,
};

struct OrderResponse {
  uint64_t m_orderId;
  ResponseType m_responseType;
};

using Clock = std::chrono::high_resolution_clock;

/////////////////////  Order /////////////////////////////////////////
namespace OrderState {

struct Resting {};
struct RequestPending {};
struct Queued {
  Clock::time_point m_queued_at;
  RequestType m_type;
};
} // namespace OrderState

template <typename T>
concept IsOrderState = std::same_as<T, OrderState::Resting> ||
                       std::same_as<T, OrderState::RequestPending> ||
                       std::same_as<T, OrderState::Queued>;

struct Order {
  using Id = uint64_t;

  // ASSUME: onData will only be called if its a new order or when state is
  // 'Resting'
  std::variant<OrderState::Resting, OrderState::RequestPending,
               OrderState::Queued>
      m_state;

  char m_side; // possible values 'B' or 'S'
  bool m_cancel_requested;

  int m_symbolId;
  double m_price;
  uint64_t m_qty;
  Id m_id;

  bool isQueued() const {
    return std::holds_alternative<OrderState::Queued>(m_state);
  }
};