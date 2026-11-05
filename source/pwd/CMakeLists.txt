cmake_minimum_required(VERSION 3.10)

project(pwd)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB SOURCES "${PROJECT_SOURCE_DIR}/source/*.cpp")

include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(pwd ${SOURCES})