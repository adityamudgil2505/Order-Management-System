cmake_minimum_required(VERSION 3.22)

project(OMS LANGUAGES "CXX")

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_GLIBCXX_DEBUG_PEDANTIC -Wall -Wextra -fsanitize=address,undefined -fno-omit-frame-pointer -Wpedantic")

add_executable(oms main.cpp 
  order_management_system.cpp 
  order_queue.cpp 
  order_request.cpp 
  order_response.cpp 
  trading_time_period.cpp
)