cmake_minimum_required(VERSION 3.23)
project(ChunkNorries)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(ChunkNorries
        ChunkTests.cpp
        frog.h)
