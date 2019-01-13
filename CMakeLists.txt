cmake_minimum_required(VERSION 3.12)
project(SchneRT)
find_package(OpenMP)
set(CMAKE_CXX_STANDARD 14)

if (OPENMP_FOUND)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

find_package(PNG REQUIRED)
include_directories(${PNG_INCLUDE_DIR})

add_executable(SchneRT main.cpp Canvas.cpp Canvas.h utils.h Stage.cpp Stage.h Object.cpp Object.h Bezier.cpp Bezier.h)

target_link_libraries(SchneRT ${PNG_LIBRARY})