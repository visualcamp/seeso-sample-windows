cmake_minimum_required(VERSION 3.5)
project(seeso_windows)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(opencv)
add_subdirectory(seeso)

add_executable(seeso_windows main.cpp
        callback.cc)

target_link_libraries(seeso_windows PUBLIC opencv seeso)


# dll copy
set(SEESO_BIN_DIR ${CMAKE_CURRENT_SOURCE_DIR}/seeso/bin)
set(OPENCV_BIN_DIR ${CMAKE_CURRENT_SOURCE_DIR}/opencv/lib/x64/vc15/bin)

# this script will work only if cmake version >= 3.5
add_custom_command(TARGET seeso_windows PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        "${SEESO_BIN_DIR}/seeso/seeso.dll"
        "${SEESO_BIN_DIR}/third-parties/libcrypto-1_1-x64.dll"
        "${SEESO_BIN_DIR}/third-parties/libcurl.dll"
        "${SEESO_BIN_DIR}/third-parties/libssl-1_1-x64.dll"
        "${OPENCV_BIN_DIR}/opencv_ffmpeg410_64.dll"
        "${OPENCV_BIN_DIR}/opencv_world410.dll"
        "${OPENCV_BIN_DIR}/opencv_world410d.dll"
        $<TARGET_FILE_DIR:seeso_windows>)
