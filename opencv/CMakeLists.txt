cmake_minimum_required(VERSION 3.5)
project(opencv)

add_library(opencv INTERFACE)

set(OPENCV_INCLUDE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(OPENCV_LIB_PATH "${CMAKE_CURRENT_SOURCE_DIR}/lib/x64/vc15/lib")

if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
  set(OPENCV_LIB "${OPENCV_LIB_PATH}/opencv_world410d.lib")
else()
  set(OPENCV_LIB "${OPENCV_LIB_PATH}/opencv_world410.lib")
endif()

target_link_libraries(opencv INTERFACE ${OPENCV_LIB})
target_include_directories(opencv INTERFACE ${OPENCV_INCLUDE_PATH})

message("OpenCV include path: ${OPENCV_INCLUDE_PATH}")
message("OpenCV library path: ${OPENCV_LIB_PATH}")