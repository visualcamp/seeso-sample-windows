cmake_minimum_required(VERSION 3.5)
project(seeso_windows)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(opencv)
add_subdirectory(seeso)

add_executable(seeso_windows main.cpp
        callback.cc
        view.cc)

target_link_libraries(seeso_windows PUBLIC opencv seeso)

# copy script will work only if cmake version >= 3.5

# copy seeso dlls
add_custom_command(TARGET seeso_windows PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${SEESO_DLL}
        $<TARGET_FILE_DIR:seeso_windows>)

#copy opencv dlls
add_custom_command(TARGET seeso_windows PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${OPENCV_DLL}
        $<TARGET_FILE_DIR:seeso_windows>)
