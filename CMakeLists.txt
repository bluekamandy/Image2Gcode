cmake_minimum_required(VERSION 3.20)

project(Image2Gcode VERSION 1.0)

# Enforce use of C++14
set(CMAKE_CXX_STANDARD_REQUIRED 20)
set(CMAKE_CXX_STANDARD 20)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file(config.h.in config.h)

set(OpenCV_DIR "$ENV{OPENCV_DIR}")

if(OpenCV_DIR)
  message(STATUS "OpenCV environment variable found")
  find_package(OpenCV REQUIRED)
else()
  message(STATUS "OpenCV environment variable not found")
endif()

# add the executable and include any header files you'd like to include.
add_executable(${PROJECT_NAME} main.cpp slicer.cpp print.cpp layer.cpp common.cpp plasticPoint.cpp slicer.hpp print.hpp layer.hpp common.hpp plasticPoint.hpp)

target_include_directories(${PROJECT_NAME}             
        PRIVATE
        ${PROJECT_BINARY_DIR}
        ${OpenCV_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME}
        PRIVATE
        ${OpenCV_LIBS}
)

# message("OpenCV Include Directories: " ${OpenCV_INCLUDE_DIRS})
# message("OpenCV Libraries: " ${OpenCV_LIBS})

target_include_directories(${PROJECT_NAME}
        PUBLIC
        ${PROJECT_BINARY_DIR}
)

target_link_directories(${PROJECT_NAME}
	PRIVATE
)

target_link_libraries(${PROJECT_NAME}
        PRIVATE
)

