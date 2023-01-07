#ifndef ORDER_RESPONSE_H
#define ORDER_RESPONSE_H
#include<cstdint>

enum class ResponseType
{
    Unknown = 0,
    Accept = 1,
    Reject = 2,
};
struct OrderResponse
{
    uint64_t m_orderId;
    ResponseType m_responseType;
};

#endif