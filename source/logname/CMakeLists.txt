cmake_minimum_required(VERSION 3.10)

project(logname)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(logname ${PROJECT_SOURCE_DIR}/source/main.cpp)