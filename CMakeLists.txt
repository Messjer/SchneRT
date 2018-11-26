cmake_minimum_required(VERSION 3.12)
project(SchneRT)

set(CMAKE_CXX_STANDARD 14)

add_executable(SchneRT main.cpp Canvas.cpp Canvas.h utils.h Stage.cpp Stage.h Object.cpp Object.h)