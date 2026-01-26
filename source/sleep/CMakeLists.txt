cmake_minimum_required(VERSION 3.10)

project(sleep)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(sleep ${PROJECT_SOURCE_DIR}/source/main.cpp)
