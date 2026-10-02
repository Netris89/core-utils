cmake_minimum_required(VERSION 3.10)

project(Utilities)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(source/echo)
add_subdirectory(source/date)
add_subdirectory(source/sleep)
add_subdirectory(source/true)
add_subdirectory(source/false)
