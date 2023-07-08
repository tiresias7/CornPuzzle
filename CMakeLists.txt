cmake_minimum_required(VERSION 3.10)
project(CornPuzzle)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include)

add_library(CornLib
    src/CornState.cpp
    src/CornSolver.cpp)

add_executable(CornPuzzle example/main.cpp)

target_link_libraries(CornPuzzle CornLib)
