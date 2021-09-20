cmake_minimum_required(VERSION 3.21)

project(majster)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-missing-field-initializers -Wsign-conversion")
set(CMAKE_EXPORT_COMPILE_COMMANDS)

add_subdirectory(src)
