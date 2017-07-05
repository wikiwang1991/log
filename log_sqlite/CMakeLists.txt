cmake_minimum_required(VERSION 2.6)
project(log_sqlite)

include_directories(../log)

add_library(log_sqlite MODULE ../log/log.def log_sqlite.cpp sqlite3.c sqlite3.h)

set_target_properties(log_sqlite PROPERTIES OUTPUT_NAME "log")
