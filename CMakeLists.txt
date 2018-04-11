cmake_minimum_required(VERSION 3.8)
project(tower_defends)

set(CMAKE_CXX_STANDARD 11)

# Create a list with all .cpp source files
set( SOURCE_FILES
        src/main.cpp src/UiItems.cpp src/UiItems.h)
add_executable(Progtest_5_1 ${SOURCE_FILES})