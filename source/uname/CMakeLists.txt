cmake_minimum_required(VERSION 3.10)

project(uname)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(uname ${PROJECT_SOURCE_DIR}/source/main.cpp)