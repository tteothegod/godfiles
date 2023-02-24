cmake_minimum_required(VERSION 3.10)

project(PmonImgReader)

find_package(OpenCV REQUIRED)
find_package(Tesseract REQUIRED)


include_directories(${OpenCV_INCLUDE_DIRS})

# Add Source File
add_executable(PmonImgReader src/main.cpp src/visual.cpp)

# Add the directory containing the header file
target_include_directories(PmonImgReader PRIVATE src)

# Link any required libraries
target_link_libraries(PmonImgReader ${OpenCV_LIBS} ${Tesseract_LIBRARIES})

# Specidy header file to include
target_sources(PmonImgReader PRIVATE src/visual.h)