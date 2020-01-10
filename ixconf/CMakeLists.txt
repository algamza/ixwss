cmake_minimum_required(VERSION 2.8)

project(ixwss_config)
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
    FILES ${CMAKE_CURRENT_SOURCE_DIR}/ixconfig.xml
    DESTINATION ${IXWSS_SDK_OUTPUT_DIR}/ixconf
    PERMISSIONS OWNER_WRITE OWNER_READ 
                GROUP_WRITE GROUP_READ 
                WORLD_WRITE WORLD_READ ) 

# install the configuration files
#install (
#    FILES ${CMAKE_CURRENT_SOURCE_DIR}/ixconfig.conf
#    DESTINATION ${IXWSS_SDK_OUTPUT_DIR}/ixconf
#    PERMISSIONS OWNER_WRITE OWNER_READ 
#                GROUP_WRITE GROUP_READ 
#                WORLD_WRITE WORLD_READ ) 
