cmake_minimum_required(VERSION 2.6)
project(log)

add_definitions(-DLOG_LEVEL=0)

add_library(log STATIC log.c log.h)

target_include_directories(log PUBLIC .)
target_link_libraries(log ${CMAKE_DL_LIBS})
