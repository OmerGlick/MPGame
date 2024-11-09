# Minimum version of CMake
cmake_minimum_required(VERSION 3.10)

# Project name and version
project(WebSocketServer VERSION 1.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Specify output directory for executables
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Find Boost libraries
find_package(Boost 1.70 REQUIRED COMPONENTS system)

# Add executable
add_executable(WebSocketServer src/websocket_server.cpp)

# Include Boost headers
target_include_directories(WebSocketServer PRIVATE ${Boost_INCLUDE_DIRS})

# Link Boost libraries
target_link_libraries(WebSocketServer PRIVATE ${Boost_LIBRARIES})
