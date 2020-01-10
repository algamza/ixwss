cmake_minimum_required(VERSION 2.8)

project(ixsamples_webclient)
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

#=====================================================================================
# install 
#=====================================================================================
# install samples
install (
    FILES ${CMAKE_CURRENT_SOURCE_DIR}/full_jsons.js
          ${CMAKE_CURRENT_SOURCE_DIR}/websocket_client.html
          ${CMAKE_CURRENT_SOURCE_DIR}/websocket_client2.html
    DESTINATION ${IXWSS_SDK_OUTPUT_DIR}/samples/client_web
    PERMISSIONS OWNER_WRITE OWNER_READ 
                GROUP_WRITE GROUP_READ 
                WORLD_WRITE WORLD_READ ) 