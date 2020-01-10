cmake_minimum_required(VERSION 2.8)

project(ixsamples_server)
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
SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -O -g")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--no-undefined -Wl,--no-allow-shlib-undefined -Wl,--no-as-needed")

include_directories(
    ${IXWSS_OPENLIBS_LIBWEBSOCKETS_HEADER_DIR}
    ${IXWSS_OPENLIBS_LIBXML2_HEADER_DIR}
    ${IXWSS_OPENLIBS_JANSSON_HEADER_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${IXWSS_LIBSRC_DIR}/wss/include
    ${IXWSS_PLUGINS_DIR} )
    
link_directories( 
    ${IXWSS_OPENLIBS_LIBWEBSOCKETS_LIB_DIR}
    ${IXWSS_OPENLIBS_LIBXML2_LIB_DIR}
    ${IXWSS_OPENLIBS_JANSSON_LIB_DIR}
    ${IXWSS_SDK_OUTPUT_DIR}/ixlib/lib )
    
set(SOURCES 
	${CMAKE_CURRENT_SOURCE_DIR}/server.cpp )

add_executable(ixs_server ${SOURCES})
target_link_libraries(ixs_server websockets ixwss xml2 jansson ssl pthread dl)

#=====================================================================================
# install 
#=====================================================================================
# install samples
install (
    DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    DESTINATION ${IXWSS_SDK_OUTPUT_DIR}/samples
    FILES_MATCHING PATTERN "*.cpp"
    PERMISSIONS OWNER_WRITE OWNER_READ 
                GROUP_WRITE GROUP_READ 
                WORLD_WRITE WORLD_READ ) 

install(
    TARGETS ixs_server 
    DESTINATION ${IXWSS_SDK_OUTPUT_DIR}/samples
    PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ 
                GROUP_EXECUTE GROUP_WRITE GROUP_READ 
                WORLD_EXECUTE WORLD_WRITE WORLD_READ 
) 



