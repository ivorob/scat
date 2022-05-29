CMAKE_MINIMUM_REQUIRED (VERSION 3.0)
PROJECT (PluginUnsupported)

SET (SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})
SET (SOURCES
        ${SRC_DIR}/PluginUnsupported.cpp
    )

SET (HEADERS
        ${SRC_DIR}/PluginUnsupported.h
    )

ADD_LIBRARY (PluginUnsupported SHARED ${SOURCES} ${HEADERS})
TARGET_LINK_LIBRARIES (PluginUnsupported pluginsdk)
