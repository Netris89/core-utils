# Set the path to the Vcpkg toolchain file
set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")

# Specify the minimum required version of CMake
cmake_minimum_required(VERSION 3.28)

# Define the project name
project(echo)

# Include CTest module to enable testing functionality
include(CTest)

# Set the C++ standard to C++20 and enforce it
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Automatically gather all .cpp source files from the 'source' directory
file(GLOB SOURCES "${PROJECT_SOURCE_DIR}/source/*.cpp")

# Add the 'include' directory to the list of header search paths
include_directories(${PROJECT_SOURCE_DIR}/include)

# Create the executable target for the main program using the gathered source files
add_executable(echo ${SOURCES})

# Enable testing in the project
enable_testing()

# Find the GoogleTest package (required for tests)
find_package(GTest REQUIRED)

# Create the test executable for parser tests
add_executable(testParser "${PROJECT_SOURCE_DIR}/test/testParser.cpp")

# Add parser.cpp directly to the test executable
target_sources(testParser PRIVATE ${PROJECT_SOURCE_DIR}/source/parser.cpp)

# Set the output directory for the test executable
set_target_properties(testParser PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build)

# Link the test executable with GoogleTest libraries
target_link_libraries(testParser PRIVATE GTest::GTest GTest::Main)

# Automatically discover tests from the test executable (requires gtest_main)
gtest_discover_tests(testParser)
