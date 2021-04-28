cmake_minimum_required(VERSION 3.20)

project(IMGtoGCODE VERSION 1.0)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file(config.h.in config.h)

set(OpenCV_DIR external/opencv/build)

find_package(OpenCV REQUIRED)

# add the executable
add_executable(${PROJECT_NAME} main.cpp)

# This file will define the following variables:
#      - OpenCV_LIBS                     : The list of all imported targets for OpenCV modules.
#      - OpenCV_INCLUDE_DIRS             : The OpenCV include directories.

target_include_directories(${PROJECT_NAME}
        PRIVATE
        ${PROJECT_BINARY_DIR}
        ${OpenCV_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME}
        PRIVATE
        ${OpenCV_LIBS})

# message("OpenCV Include Directories: " ${OpenCV_INCLUDE_DIRS})
# message("OpenCV Libraries: " ${OpenCV_LIBS})

