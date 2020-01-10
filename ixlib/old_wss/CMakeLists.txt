cmake_minimum_required(VERSION 2.8)

project(ixwsslib)

set(IXWSS_OUTPUT_DIR "${IXWSS_SDK_OUTPUT_DIR}/ixlib")

message(STATUS "CMAKE_CURRENT_SOURCE_DIR=" ${CMAKE_CURRENT_SOURCE_DIR})
message(STATUS "IXWSS_OUTPUT_DIR=" ${IXWSS_OUTPUT_DIR})

#=======================================================================================
# options
#=======================================================================================

#=======================================================================================
# defines 
#=======================================================================================

#=====================================================================================
# define directory path
#=====================================================================================

#=======================================================================================
# build sources
#=======================================================================================
set(LIBRARY_OUTPUT_PATH ${IXWSS_OUTPUT_DIR}/lib)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/include/IXWebSocketServer.h DESTINATION ${IXWSS_OUTPUT_DIR}/include)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${IXWSS_OPENLIBS_LIBWEBSOCKETS_HEADER_DIR}
    ${IXWSS_PLUGINS_DIR}
)

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/IXThread.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/IXWebSocketData.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/IXWebSocketSetting.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/IXWebSocketPluginSetting.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/IXWebSocketSession.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/IXWebSocketVhost.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/IXWebSocketServer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/IXWebSocketServerPrivate.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/IXPluginAdapter.cpp
)

add_library(ixwss SHARED ${SOURCES})
INSTALL(TARGETS ixwss DESTINATION ${IXWSS_OUTPUT_DIR}/lib)

#doxygen generation
add_custom_command(
    TARGET ixwss
    POST_BUILD
    COMMENT "Doxyfile Generation"
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMAND doxygen "${CMAKE_SOURCE_DIR}/doxyconf"
)

#version information generation
add_custom_command(
    TARGET ixwss
    POST_BUILD
    COMMENT "Version Information Generated"
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMAND echo ${IXWSS_SDK_VERSION} > ${CMAKE_BINARY_DIR}/version.txt
    COMMAND cp ${CMAKE_BINARY_DIR}/version.txt ${IXWSS_SDK_OUTPUT_DIR}
)
