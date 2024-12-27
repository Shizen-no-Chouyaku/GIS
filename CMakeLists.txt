# CMakeLists.txt

cmake_minimum_required(VERSION 3.10)
project(CustomGIS)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Find required packages using pkg-config
find_package(PkgConfig REQUIRED)

# Find SDL2
pkg_check_modules(SDL2 REQUIRED sdl2)
include_directories(${SDL2_INCLUDE_DIRS})

# Find SDL2_image
pkg_check_modules(SDL2_IMAGE REQUIRED SDL2_image)
include_directories(${SDL2_IMAGE_INCLUDE_DIRS})

# Find CURL
pkg_check_modules(CURL REQUIRED libcurl)
include_directories(${CURL_INCLUDE_DIRS})

# Specify the source directories
file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "src/*.h")

# Add executable
add_executable(CustomGIS ${SOURCES} ${HEADERS})

# Link libraries
target_link_libraries(CustomGIS 
    ${SDL2_LIBRARIES} 
    ${SDL2_IMAGE_LIBRARIES} 
    ${CURL_LIBRARIES}
    pthread
    # Add any other necessary libraries here
)
