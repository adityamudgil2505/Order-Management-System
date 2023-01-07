#include"order_queue.hpp"

OrderQueue::OrderQueue(): size_(0){
}

OrderRequest OrderQueue::pop(){
    std::unique_lock<std::mutex> locker(this->mu_);
    this->cond_.wait(locker, [&](){return !this->q_.empty();});
    OrderRequest& res = this->q_.front();
    this->q_.pop_front();
    this->size_--;
    locker.unlock();
    return res;
}
void OrderQueue::push(OrderRequest& order){
    std::lock_guard<std::mutex> locker = std::lock_guard<std::mutex>(this->mu_);
    this->q_.push_back(order);
    size_++;
    this->cond_.notify_one();
}

void OrderQueue::push(OrderRequest&& order){
    std::lock_guard<std::mutex> locker = std::lock_guard<std::mutex>(this->mu_);
    this->q_.push_back(std::move(order));
    size_++;
    this->cond_.notify_one();
}

void OrderQueue::update_order(OrderRequest& order){
    if(this->hmap_.count(order.m_orderId)){
        this->hmap_[order.m_orderId]->m_price = order.m_price;
        this->hmap_[order.m_orderId]->m_qty = order.m_qty;
    }
}

void OrderQueue::remove_order(const uint64_t& order_id){
    if(this->hmap_.count(order_id)){
        std::lock_guard<std::mutex> locker = std::lock_guard<std::mutex>(this->mu_);
        this->q_.erase(this->hmap_[order_id]);
        this->hmap_.erase(order_id);
        this->size_--;
    }
}
bool OrderQueue::is_empty(){
    return bool(this->q_.empty());
}

void OrderQueue::handle_request(OrderRequest& order, const RequestType& type){
    switch(type){
        case RequestType::Unknown:
            break;
        case RequestType::New:
            this->push(order);
            break;
        case RequestType::Modify:
            this->update_order(order);
            break;
        case RequestType::Cancel:
            this->remove_order(order.m_orderId);
            break;
        default:
            break;
    };
}