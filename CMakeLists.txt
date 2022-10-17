CMAKE_MINIMUM_REQUIRED (VERSION 3.0)
PROJECT (scat)

SET (SRC_DIR src)
SET (INCLUDE_DIR src)
SET (SOURCES 
        ${SRC_DIR}/main.cpp
        ${SRC_DIR}/TestFileParser.cpp
        ${SRC_DIR}/TestFileParseException.cpp
        ${SRC_DIR}/ScenarioTokenizer.cpp
        ${SRC_DIR}/Token.cpp
        ${SRC_DIR}/TestScenario.cpp
    )

SET (HEADERS
        ${INCLUDE_DIR}/TestFileParser.h
        ${INCLUDE_DIR}/TestFileParseException.h
        ${INCLUDE_DIR}/ScenarioTokenizer.h
        ${INCLUDE_DIR}/Token.h
        ${INCLUDE_DIR}/TestScenario.h
    )

OPTION (BUILD_TESTING "Build testing" OFF)

SET (CMAKE_CXX_STANDARD 17)

ADD_SUBDIRECTORY (thirdparty)
ADD_SUBDIRECTORY (sdk)
ADD_SUBDIRECTORY (plugins)

IF (BUILD_TESTING)
    ENABLE_TESTING ()
    ADD_SUBDIRECTORY (tests tests)

    ADD_TEST (tests tests/tests)
ENDIF ()

ADD_EXECUTABLE (scat ${SOURCES})
