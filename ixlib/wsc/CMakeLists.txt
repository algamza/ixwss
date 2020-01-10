cmake_minimum_required(VERSION 2.8)

project(ixwsclib)
message (STATUS "..proc = ${PROJECT_NAME}") 
#=====================================================================================
# define directory path
#=====================================================================================

#=====================================================================================
# sub directories
#=====================================================================================

#=====================================================================================
# build sources
#=====================================================================================
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../wss/include
    ${IXWSS_OPENLIBS_LIBWEBSOCKETS_HEADER_DIR}
    ${IXWSS_OPENLIBS_LIBXML2_HEADER_DIR}
    ${IXWSS_PLUGINS_DIR}
)

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/IXClientThread.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/IXWebSocketClientData.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/IXWebSocketClient.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/IXWebSocketClientPrivate.cpp
)

add_library(ixwsc SHARED ${SOURCES})
set_target_properties(ixwsc PROPERTIES VERSION ${IXWSS_SDK_VERSION}
                                       SOVERSION ${IXWSS_SDK_VERSION_MAJOR} )


#doxygen generation
add_custom_command(
    TARGET ixwsc
    POST_BUILD
    COMMENT "Doxyfile Generation"
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMAND doxygen "${CMAKE_SOURCE_DIR}/doxyconf"
)

#=====================================================================================
# install 
#=====================================================================================
install(
    FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/IXWebSocketClient.h 
    DESTINATION ${IXWSS_SDK_OUTPUT_DIR}/ixlib/include
    PERMISSIONS OWNER_WRITE OWNER_READ 
                GROUP_WRITE GROUP_READ 
                WORLD_WRITE WORLD_READ 
) 

install(
    TARGETS ixwsc 
    DESTINATION ${IXWSS_SDK_OUTPUT_DIR}/ixlib/lib
    PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ 
                GROUP_EXECUTE GROUP_WRITE GROUP_READ 
                WORLD_EXECUTE WORLD_WRITE WORLD_READ 
) 
