#ifndef TRADING_TIME_PERIOD_H
#define TRADING_TIME_PERIOD_H

#include <chrono>

struct TradingTimePeriod{
    std::chrono::high_resolution_clock start_time;
    std::chrono::high_resolution_clock end_time;
};

#endif