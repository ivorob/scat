CMAKE_MINIMUM_REQUIRED (VERSION 2.8)
PROJECT (tests)

SET (MAIN_SRC_DIR ${CMAKE_SOURCE_DIR}/src)
SET (MAIN_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
SET (SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})
SET (INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
SET (SOURCES 
        tests.cpp
        ${SRC_DIR}/TestCommandTest.cpp
        ${SRC_DIR}/TestFileParserTest.cpp
        # main source files
        ${MAIN_SRC_DIR}/TestCommand.cpp
        ${MAIN_SRC_DIR}/TestFileParser.cpp
    )

SET (HEADERS
        ${MAIN_SRC_DIR}/TestCommand.h
        ${MAIN_SRC_DIR}/TestFileParser.h
    )

ADD_EXECUTABLE (tests ${SOURCES} ${HEADERS})
TARGET_LINK_LIBRARIES (tests gtest)
TARGET_INCLUDE_DIRECTORIES (tests PRIVATE ${INCLUDE_DIR})
TARGET_INCLUDE_DIRECTORIES (tests PRIVATE ${MAIN_SRC_DIR})

ADD_CUSTOM_TARGET (JUnitTestReport
    COMMAND $<TARGET_FILE:tests> --gtest_output="xml:$ENV{BINARY_DIR}/tests.xml"
    DEPENDS tests
)

IF (WIN32)
    TARGET_LINK_LIBRARIES (tests ws2_32)
ENDIF ()
