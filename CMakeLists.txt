cmake_minimum_required(VERSION 3.10)

# Project name and version
project(img_lib_system VERSION 1.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/img_lib)
include_directories(${CMAKE_SOURCE_DIR}/img_lib/test)

# Add sources
set(SOURCES
    main.cpp
    load_image.cpp
    test/process_image.cpp
)

# Add header files for IDEs (optional, helps IDEs find headers)
set(HEADERS
    image.hpp
    stb_image.h
    stb_image_write.h
)

# Add executable
add_executable(img_lib_system ${SOURCES} ${HEADERS})

