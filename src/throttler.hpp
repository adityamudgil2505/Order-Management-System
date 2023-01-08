#pragma once

#include "common.hpp"
#include <queue>

class Throttler {
  constexpr static int MAX_REQ = 100;
  // poor man's circular buffer
  std::deque<Clock::time_point> m_reqs_sent;

public:
  bool canSend() {
    const auto sec =
        Clock::time_point{std::chrono::duration_cast<std::chrono::seconds>(
            Clock::now().time_since_epoch())};
    while (!m_reqs_sent.empty() && m_reqs_sent.front() <= sec) {
      m_reqs_sent.pop_front();
    }
    return m_reqs_sent.size() < MAX_REQ;
  }

  void notify_req_sent(Clock::time_point tp) { m_reqs_sent.push_back(tp); }
};