find_package(GTest QUIET)

if(NOT GTest_FOUND)
    message(FATAL_ERROR "The unit tests need GoogleTest (for example installed with vcpkg or with your package manager). "
                        "Install it, or configure without -DBUILD_TESTING=ON")
endif()

include(GoogleTest)

add_executable(dateTestParser date/testParser.cpp ${PROJECT_SOURCE_DIR}/source/date/source/parser.cpp ${PROJECT_SOURCE_DIR}/source/date/source/clock.cpp)
add_executable(echoTestParser echo/testParser.cpp ${PROJECT_SOURCE_DIR}/source/echo/source/parser.cpp)

target_include_directories(dateTestParser PRIVATE ${PROJECT_SOURCE_DIR}/source/date/include)
target_include_directories(echoTestParser PRIVATE ${PROJECT_SOURCE_DIR}/source/echo/include)

target_link_libraries(dateTestParser PRIVATE GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main)
target_link_libraries(echoTestParser PRIVATE GTest::gtest GTest::gtest_main)

gtest_discover_tests(dateTestParser)
gtest_discover_tests(echoTestParser)