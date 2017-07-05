cmake_minimum_required(VERSION 3.1)
project(log_txt)

set(CMAKE_CXX_STANDARD 11)

include_directories(../log)

add_library(log_txt MODULE ../log/log.def log_txt.cpp)

set_target_properties(log_txt PROPERTIES OUTPUT_NAME "log")
