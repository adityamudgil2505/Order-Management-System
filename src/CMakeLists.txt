add_library(oms STATIC
  oms.cpp oms.hpp
  throttler.hpp
  common.hpp
)
target_include_directories(oms INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})