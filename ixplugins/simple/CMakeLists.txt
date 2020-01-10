cmake_minimum_required(VERSION 2.8)

project(ixplugins_simple)
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
    ${CMAKE_CURRENT_SOURCE_DIR}/modules
    ${IXWSS_PLUGINS_DIR}
)

file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_library(ixplugin_simple SHARED ${SOURCES})

#=====================================================================================
# install 
#=====================================================================================

install(
    TARGETS ixplugin_simple 
    DESTINATION ${IXWSS_SDK_OUTPUT_DIR}/ixplugins/bin
    PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ 
                GROUP_EXECUTE GROUP_WRITE GROUP_READ 
                WORLD_EXECUTE WORLD_READ 
) 

# install the ixplugin examples
install (
    DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    DESTINATION ${IXWSS_SDK_OUTPUT_DIR}/ixplugins
    FILES_MATCHING PATTERN "*.cpp" PATTERN "*.h"
    PERMISSIONS OWNER_WRITE OWNER_READ 
                GROUP_WRITE GROUP_READ 
                WORLD_WRITE WORLD_READ ) 

install (
    FILES ${CMAKE_CURRENT_SOURCE_DIR}/../ixplugin.h
    DESTINATION ${IXWSS_SDK_OUTPUT_DIR}/ixplugins/simple
    PERMISSIONS OWNER_WRITE OWNER_READ 
                GROUP_WRITE GROUP_READ 
                WORLD_WRITE WORLD_READ ) 
