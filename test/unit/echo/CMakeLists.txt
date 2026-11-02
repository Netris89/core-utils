add_unit_test(echoTestParser
    testParser.cpp
    ${PROJECT_SOURCE_DIR}/source/echo/source/parser.cpp
)

target_include_directories(echoTestParser
    PRIVATE
        ${PROJECT_SOURCE_DIR}/source/echo/include
)
