cmake_minimum_required(VERSION 2.6)
project(log)

add_definitions(-DLOG_LEVEL=0)

add_library(log STATIC log.c log.h)

if (CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64")
  SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "-fPIC")
endif ()

target_include_directories(log PUBLIC .)
target_link_libraries(log ${CMAKE_DL_LIBS})
