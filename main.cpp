#include"order_management_system.hpp"
#include"order_queue.hpp"
#include"order_request.hpp"

void upstream1(OrderQueue& order_queue){
    std::cout<<&order_queue<<"\n";
    OrderRequest order1(1, 10, 300, 'B', 1001);
    OrderRequest order2(2, 32, 100, 'B', 1002);
    OrderRequest order3(3, 10, 300, 'B', 1003);
    OrderRequest order4(0, 0, 0, 'B', 1001);
    OrderRequest order5(3, 14, 50, 'B', 1002);
    order_queue.handle_request(order1, RequestType::New);
    order_queue.handle_request(order2, RequestType::New);
    order_queue.handle_request(order3, RequestType::New);
    order_queue.handle_request(order4, RequestType::Cancel);
    order_queue.handle_request(order5, RequestType::Modify);
}

void upstream2(OrderQueue& order_queue){
    OrderRequest order1(1, 10, 300, 'B', 2001);
    OrderRequest order2(2, 32, 100, 'B', 2002);
    OrderRequest order3(3, 10, 300, 'B', 2001);
    OrderRequest order4(1, 10, 300, 'B', 2001);
    OrderRequest order5(1, 10, 300, 'B', 2001);

    order_queue.handle_request(order1, RequestType::New);
    order_queue.handle_request(order2, RequestType::New);
    order_queue.handle_request(order3, RequestType::Cancel);
    order_queue.handle_request(order4, RequestType::Cancel);
    order_queue.handle_request(order5, RequestType::Modify);
}

int main(){
    OrderManagementSystem order_management_system("admin", "password");
    OrderQueue& order_queue = order_management_system.getOrderQueue();
    std::cout<<&order_queue<<"\n";
    
    std::thread client1(upstream1, std::ref(order_queue));
    std::thread client2(upstream2, std::ref(order_queue));
    
    order_management_system.requestGateway();

    if(client1.joinable())  client1.join();
    if(client2.joinable())  client2.join();

    return EXIT_SUCCESS;
}