cmake_minimum_required(VERSION 2.6)
project(log)

add_definitions(-DLOG_LEVEL=0)

add_library(log STATIC log.c log.h)
