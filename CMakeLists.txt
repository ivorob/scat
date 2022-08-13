CMAKE_MINIMUM_REQUIRED (VERSION 3.0)
PROJECT (scat)

SET (SRC_DIR src)
SET (INCLUDE_DIR src)
SET (SOURCES 
        ${SRC_DIR}/main.cpp
        ${SRC_DIR}/TestFileParser.cpp
    )

SET (HEADERS
        ${INCLUDE_DIR}/TestFileParser.h
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
