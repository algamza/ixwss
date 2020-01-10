cmake_minimum_required(VERSION 2.8)

project(ixwss_cert)
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
# install the configuration files
install (
    FILES ${CMAKE_CURRENT_SOURCE_DIR}/ixserver.key 
          ${CMAKE_CURRENT_SOURCE_DIR}/rootca.crt
          ${CMAKE_CURRENT_SOURCE_DIR}/ixserver.crt
    DESTINATION ${IXWSS_SDK_OUTPUT_DIR}/ixcert
    PERMISSIONS OWNER_WRITE OWNER_READ 
                GROUP_WRITE GROUP_READ 
                WORLD_WRITE WORLD_READ ) 