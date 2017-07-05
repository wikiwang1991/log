cmake_minimum_required(VERSION 3.1)
project(log_sqlite)

set(CMAKE_CXX_STANDARD 11)

add_definitions(-DSQLITE_THREADSAFE=0)

include_directories(../log)

add_library(log_sqlite MODULE ../log/log.def log_sqlite.cpp sqlite3.c sqlite3.h)

set_target_properties(log_sqlite PROPERTIES OUTPUT_NAME "log")
