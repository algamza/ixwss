cmake_minimum_required(VERSION 2.8)

project(ixwsslib)
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
    ${IXWSS_OPENLIBS_LIBWEBSOCKETS_HEADER_DIR}
    ${IXWSS_OPENLIBS_LIBXML2_HEADER_DIR}
    ${IXWSS_PLUGINS_DIR}
)

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/IxConfig.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/IxConfigXML.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/IxConfigPort.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/IxConfigProtocol.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/IxConnectionInfo.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/IxHttpSessionData.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/IxPluginConnector.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/IxPluginManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/IxSession.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/IxSessionData.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/IxThread.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/IxWebSocketServer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/IxWebSocketSessionData.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/IxWebSocketVhost.cpp
)

add_library(ixwss SHARED ${SOURCES})
set_target_properties(ixwss PROPERTIES VERSION ${IXWSS_SDK_VERSION}
                                       SOVERSION ${IXWSS_SDK_VERSION_MAJOR} )

#doxygen generation
add_custom_command(
    TARGET ixwss
    POST_BUILD
    COMMENT "Doxyfile Generation"
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMAND doxygen "${CMAKE_SOURCE_DIR}/doxyconf"
)

#=====================================================================================
# install 
#=====================================================================================

install (
    TARGETS ixwss 
    DESTINATION ${IXWSS_SDK_OUTPUT_DIR}/ixlib/lib 
    PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ 
                GROUP_EXECUTE GROUP_WRITE GROUP_READ 
                WORLD_EXECUTE WORLD_WRITE WORLD_READ 
) 

install (
    FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/IxWebSocketServer.h
          ${CMAKE_CURRENT_SOURCE_DIR}/include/IxServer.h
          ${CMAKE_CURRENT_SOURCE_DIR}/include/IxThread.h
    DESTINATION ${IXWSS_SDK_OUTPUT_DIR}/ixlib/include
    PERMISSIONS OWNER_WRITE OWNER_READ 
                GROUP_WRITE GROUP_READ 
                WORLD_WRITE WORLD_READ 
) 

