CMAKE_MINIMUM_REQUIRED (VERSION 3.0)
PROJECT (tests)

SET (MAIN_SRC_DIR ${CMAKE_SOURCE_DIR}/src)
SET (MAIN_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/src)
SET (SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})
SET (INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
SET (SOURCES 
        tests.cpp
        ${SRC_DIR}/TestFileParserTest.cpp
        ${SRC_DIR}/ScenarioTokenizerTest.cpp
        # main source files
        ${MAIN_SRC_DIR}/TestScenario.cpp
        ${MAIN_SRC_DIR}/TestFileParser.cpp
        ${MAIN_SRC_DIR}/TestFileParseException.cpp
        ${MAIN_SRC_DIR}/ScenarioTokenizer.cpp
        ${MAIN_SRC_DIR}/Token.cpp
    )

SET (HEADERS
        ${MAIN_INCLUDE_DIR}/TestFileParser.h
        ${MAIN_INCLUDE_DIR}/TestScenario.h
        ${MAIN_INCLUDE_DIR}/TestFileParseException.h
        ${MAIN_INCLUDE_DIR}/ScenarioTokenizer.h
        ${MAIN_INCLUDE_DIR}/Token.h
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
