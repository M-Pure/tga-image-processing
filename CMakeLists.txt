cmake_minimum_required(VERSION 4.1)
project(Project_2)

set(CMAKE_CXX_STANDARD 20)

add_executable(Project_2 src/main.cpp
        src/Header.h
        src/Pixel.cpp
        src/Pixel.h
        src/FileOperations.h
        src/Header.cpp
        src/FileOperations.cpp
        src/Image.cpp
        src/Image.h)
