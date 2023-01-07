#ifndef ORDER_REQUEST_H
#define ORDER_REQUEST_H

#include<cstdint>

struct OrderRequest
{
    int m_symbolId;
    double m_price;
    uint64_t m_qty;
    char m_side; // possible values 'B' or 'S'
    uint64_t m_orderId;
    OrderRequest(int symbol_id, double price, uint64_t qty, char side, uint64_t order_id):m_symbolId{symbol_id}, m_price{price}, m_qty{qty}, m_side{side}, m_orderId{order_id}{
    }
};
enum class RequestType
{
    Unknown = 0,
    New = 1,
    Modify = 2,
    Cancel = 3
};

#endif