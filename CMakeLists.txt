cmake_minimum_required(VERSION 3.22)

project(OMS LANGUAGES "CXX")

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_GLIBCXX_DEBUG_PEDANTIC -Wall -Wextra -fsanitize=address,undefined -fno-omit-frame-pointer -Wpedantic")

add_subdirectory(src)
add_subdirectory(tests)