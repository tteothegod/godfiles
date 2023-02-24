cmake_minimum_required(VERSION 3.10)

project(opencv)

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

# Add Source File
add_executable(opencv src/main.cpp src/visual.cpp)

# Add the directory containing the header file
target_include_directories(opencv PRIVATE src)

# Link any required libraries
target_link_libraries(opencv ${OpenCV_LIBS})

# Specidy header file to include
target_sources(opencv PRIVATE src/visual.h)

