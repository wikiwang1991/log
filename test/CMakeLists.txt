cmake_minimum_required(VERSION 3.1)
project(test)

set(CMAKE_CXX_STANDARD 11)

add_definitions(-DLOG_LEVEL=0)

add_executable(log_test main.cpp)

target_link_libraries(log_test log ${CMAKE_DL_LIBS})
