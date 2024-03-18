# Set the minimum version of CMake that can be used
# To find the cmake version run
# $ cmake --version
cmake_minimum_required(VERSION 3.5)

# Set the project name
add_subdirectory(abc)
project (sasimi-vecbee)

# Add include paths
include_directories(${PROJECT_SOURCE_DIR}/abc/src)
include_directories(${PROJECT_SOURCE_DIR}/inc)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set (CXX_STANDARD 17) 
# Add definitions
add_definitions(-DLIN64 -g -Wall -O3 -std=c++17)

# set output path
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR})

# Add an executable with the above sources
add_executable(sasimi-vecbee.out src/main.cc src/simulator.cc src/sasimi.cc src/cktUtil.cc)

# link libraries
target_link_libraries(sasimi-vecbee.out libabc -lm -ldl -rdynamic -lreadline -ltermcap -lpthread -lstdc++ -lrt -lstdc++fs)

add_executable(profiler src/profiler.cc src/simulator.cc src/sasimi.cc src/cktUtil.cc)
target_link_libraries(profiler libabc -lm -ldl -rdynamic -lreadline -ltermcap -lpthread -lstdc++ -lrt -lstdc++fs)

