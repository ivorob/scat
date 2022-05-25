CMAKE_MINIMUM_REQUIRED (VERSION 2.8)
PROJECT (tests)

SET (SRC_DIR ${CMAKE_SOURCE_DIR}/src)
SET (INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
SET (SOURCES 
        tests.cpp
    )

SET (HEADERS
    )

ADD_EXECUTABLE (tests ${SOURCES})
TARGET_LINK_LIBRARIES (tests gtest)
TARGET_INCLUDE_DIRECTORIES (tests PRIVATE ${INCLUDE_DIR})

ADD_CUSTOM_TARGET (JUnitTestReport
    COMMAND $<TARGET_FILE:tests> --gtest_output="xml:$ENV{BINARY_DIR}/tests.xml"
    DEPENDS tests
)

IF (WIN32)
    TARGET_LINK_LIBRARIES (tests ws2_32)
ENDIF ()