cmake_minimum_required(VERSION 2.8)

project(ixsamples_server)

set(IXSAMPLES_SERVER_OUTPUT_DIR "${IXWSS_SDK_OUTPUT_DIR}/samples/")

message(STATUS "CMAKE_CURRENT_SOURCE_DIR=" ${CMAKE_CURRENT_SOURCE_DIR})

message(STATUS "CMAKE_CURRENT_SOURCE_DIR=" ${CMAKE_CURRENT_SOURCE_DIR})
#=================================================================================================
# options
#=================================================================================================

#=================================================================================================
# defines 
#=================================================================================================

#=================================================================================================
# build sources
#=================================================================================================
SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -O -g")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--allow-shlib-undefined -Wl,--no-as-needed")
set(EXECUTABLE_OUTPUT_PATH ${IXSAMPLES_SERVER_OUTPUT_DIR})

include_directories(
    ${IXWSS_OPENLIBS_LIBWEBSOCKETS_HEADER_DIR}
    ${IXWSS_OPENLIBS_JANSSON_HEADER_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${IXWSS_LIBSRC_DIR}/wss/include
    ${IXWSS_PLUGINS_DIR}
)

link_directories( 
    ${IXWSS_OPENLIBS_LIBWEBSOCKETS_LIB_DIR}
    ${IXWSS_OPENLIBS_LIBXML2_LIB_DIR}
    ${IXWSS_OPENLIBS_JANSSON_LIB_DIR}
    ${IXWSS_SDK_OUTPUT_DIR}/ixlib/lib/
)

set(SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/server.cpp
)

add_executable(ixwss_server ${SOURCES})

target_link_libraries(ixwss_server ixwss websockets pthread jansson ssl)
