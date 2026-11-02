add_unit_test(dateTestParser
    testParser.cpp
    ${PROJECT_SOURCE_DIR}/source/date/source/parser.cpp
    ${PROJECT_SOURCE_DIR}/source/date/source/clock.cpp
)

target_include_directories(dateTestParser
    PRIVATE
        ${PROJECT_SOURCE_DIR}/source/date/include
)

target_link_libraries(dateTestParser
    PRIVATE
        GTest::gmock
        GTest::gmock_main
)
