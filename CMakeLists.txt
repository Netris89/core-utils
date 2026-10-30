# Use vcpkg only when it is installed, so that a compiler and CMake are enough to build the utilities.
# This must be done before the first project() call
if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE
   AND EXISTS "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")
endif()

cmake_minimum_required(VERSION 3.10)

project(Utilities)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# The unit tests need GoogleTest, so they are not built unless asked for (-DBUILD_TESTING=ON).
# The option must be declared before include(CTest), which would otherwise turn it on
option(BUILD_TESTING "Build the unit tests (requires GoogleTest)" OFF)
include(CTest)

# Code coverage is only supported with Clang (llvm-cov), so it is not built unless asked for (-DENABLE_COVERAGE=ON)
option(ENABLE_COVERAGE "Instrument the programs for llvm-cov (requires Clang)" OFF)

if(ENABLE_COVERAGE)
    if(NOT CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        message(FATAL_ERROR "ENABLE_COVERAGE requires Clang, the compiler is ${CMAKE_CXX_COMPILER_ID}")
    endif()

    add_compile_options(-fprofile-instr-generate -fcoverage-mapping)
    add_link_options(-fprofile-instr-generate)
endif()

add_subdirectory(source/date)
add_subdirectory(source/echo)
add_subdirectory(source/false)
add_subdirectory(source/logname)
add_subdirectory(source/sleep)
add_subdirectory(source/true)
add_subdirectory(source/tty)
add_subdirectory(source/uname)

if(BUILD_TESTING)
    add_subdirectory(test)
endif()