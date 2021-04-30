cmake_minimum_required(VERSION 3.20)

project(IMGtoGCODE VERSION 1.0)

# Enforce use of C++14
set(CMAKE_CXX_STANDARD_REQUIRED 20)
set(CMAKE_CXX_STANDARD 20)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file(config.h.in config.h)

set(OpenCV_DIR external/opencv/build)

find_package(OpenCV REQUIRED)

# add the executable and include any header files you'd like to include.
add_executable(${PROJECT_NAME} main.cpp slicer.cpp  mesh.cpp layer.cpp slicer.hpp mesh.hpp layer.hpp)

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

add_subdirectory(external/glm)

target_include_directories(${PROJECT_NAME}
        PUBLIC
        ${PROJECT_BINARY_DIR}
        external/glm/glm 
)

target_link_directories(${PROJECT_NAME}
	PRIVATE
        external/glm/glm
)

target_link_libraries(${PROJECT_NAME}
        PRIVATE
        glm::glm
)

