# Find GoogleTest, which is required to build and run the tests.
find_package(GTest QUIET)

if(NOT GTest_FOUND)
    message(FATAL_ERROR
        "The tests need GoogleTest (for example installed with vcpkg or your package manager). "
        "Install it, or configure without -DBUILD_TESTING=ON."
    )
endif()

# Load GoogleTest's CMake integration and enable CTest.
include(GoogleTest)
enable_testing()

# Create a unit test executable and register its tests with CTest.
function(add_unit_test TEST_NAME)
    add_executable(${TEST_NAME} ${ARGN})

    target_link_libraries(${TEST_NAME}
        PRIVATE
            GTest::gtest
            GTest::gtest_main
    )

    gtest_discover_tests(${TEST_NAME})
endfunction()

# Create an integration test executable and register its tests with CTest.
function(add_integration_test TEST_NAME)
    add_executable(${TEST_NAME} ${ARGN})

    target_link_libraries(${TEST_NAME}
        PRIVATE
            GTest::gtest_main
    )

    gtest_discover_tests(${TEST_NAME}
        PROPERTIES
            WORKING_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
    )
endfunction()

# Automatically discover and include all utility test directories.
foreach(TEST_TYPE unit integration)
    file(GLOB TESTS CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/${TEST_TYPE}/*"
    )

    foreach(TEST ${TESTS})
        if(IS_DIRECTORY "${TEST}" AND EXISTS "${TEST}/CMakeLists.txt")
            add_subdirectory("${TEST}")
        endif()
    endforeach()
endforeach()
