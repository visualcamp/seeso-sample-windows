cmake_minimum_required(VERSION 3.5)
project(seeso)

set(CMAKE_CXX_STANDARD 14)

set(SEESO_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(SEESO_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include/seeso)

add_library(seeso STATIC
        ${SEESO_DIR}/eye_tracker.cc
        ${SEESO_DIR}/framework/core_callback.cc
        ${SEESO_DIR}/framework/user_status_option.cc
        ${SEESO_DIR}/util/coord_converter.cc
        ${SEESO_DIR}/util/display.cc
        )

target_include_directories(seeso PUBLIC
        ${SEESO_INCLUDE_DIR})
