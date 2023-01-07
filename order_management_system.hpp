#ifndef ORDER_MANAGEMENT_SYSTEM_H
#define ORDER_MANAGEMENT_SYSTEM_H

#include<chrono>
#include<string>
#include<thread>
#include"time.h"
#include"sys/time.h"
#include"order_request.hpp"
#include"order_response.hpp"
#include"trading_time_period.hpp"
#include"order_queue.hpp"

class OrderManagementSystem{
private:
    std::string username_;
    std::string password_;
    OrderQueue order_queue_;
    uint32_t max_processing_rate;
    long min_time_;
    long long last_action_ms;
    

public:
    OrderManagementSystem(std::string username, std::string password);
    void onData(OrderRequest && request);
    void onData(OrderResponse &&response);
    void send(const OrderRequest &request);
    void sendLogon();
    void sendLogout();
    void loadData();
    void requestGateway();
    OrderQueue& getOrderQueue();
};

#endif