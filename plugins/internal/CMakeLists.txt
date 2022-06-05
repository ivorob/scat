CMAKE_MINIMUM_REQUIRED (VERSION 3.0)
PROJECT (InternalPlugins)

SET (SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})
SET (SOURCES
        ${SRC_DIR}/PluginUnsupported.cpp
        ${SRC_DIR}/InternalPluginFactory.cpp
    )

SET (HEADERS
        ${SRC_DIR}/PluginUnsupported.h
        ${SRC_DIR}/InternalPluginFactory.h
    )

ADD_LIBRARY (InternalPlugins STATIC ${SOURCES} ${HEADERS})
TARGET_INCLUDE_DIRECTORIES (InternalPlugins PUBLIC ${SRC_DIR})
TARGET_LINK_LIBRARIES (InternalPlugins pluginsdk)
