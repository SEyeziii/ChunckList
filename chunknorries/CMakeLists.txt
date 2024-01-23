cmake_minimum_required(VERSION 3.23)
project(Chunkinzzz_main)

set(CMAKE_CXX_STANDARD 14)

include_directories(ChunkNorries)

add_executable(Chunkinzzz_main
        ChunkNorries/ChunkTests.cpp
        ChunkNorries/frog.h)
