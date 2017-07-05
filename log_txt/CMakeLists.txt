cmake_minimum_required(VERSION 2.6)
project(log_txt)

include_directories(../log)

add_library(log_txt MODULE ../log/log.def log_txt.cpp)

set_target_properties(log_txt PROPERTIES OUTPUT_NAME "log")
