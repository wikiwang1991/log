cmake_minimum_required(VERSION 3.1)
project(log_server)

set(CMAKE_CXX_STANDARD 11)

include_directories(../log)

add_library(log_server MODULE ../log/log.def log_server.cpp)

set_target_properties(log_server PROPERTIES OUTPUT_NAME "log")