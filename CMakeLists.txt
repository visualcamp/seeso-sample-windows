cmake_minimum_required(VERSION 3.5)
project(seeso_windows)

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(opencv)
add_subdirectory(seeso)

add_executable(seeso_windows main.cpp
        tracker_manager.cc
        camera_thread.cc
        view.cc
        priority_mutex.cc)

target_link_libraries(seeso_windows PUBLIC opencv seeso)

if (DEFINED SEESO_TEST_KEY)
    ADD_DEFINITIONS(-DSEESO_TEST_KEY=${SEESO_TEST_KEY})
endif()

if(WIN32)
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
else()
    message(AUTHOR_WARNING "SeeSo : Running on non-Windows is experimental")
endif()
