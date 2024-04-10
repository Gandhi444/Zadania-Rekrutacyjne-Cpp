cmake_minimum_required(VERSION 3.28)
project(Colors)

set(CMAKE_CXX_STANDARD 17)

add_executable(Colors main.cpp
        Color.cpp
        Color.h)
