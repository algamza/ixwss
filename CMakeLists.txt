cmake_minimum_required(VERSION 2.8)

project(ixwss)

#=====================================================================================
# Version
#=====================================================================================
set( IXWSS_SDK_VERSION_MAJOR "1" )
set( IXWSS_SDK_VERSION_MINOR "1" )
set( IXWSS_SDK_VERSION_PATCH "0" )
set( IXWSS_SDK_VERSION "${IXWSS_SDK_VERSION_MAJOR}.${IXWSS_SDK_VERSION_MINOR}.${IXWSS_SDK_VERSION_PATCH}" )

#=====================================================================================
# Build Options
#=====================================================================================
message( STATUS "CMAKE_TOOLCHAIN_FILE='${CMAKE_TOOLCHAIN_FILE}'" )

set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -O2" )
set( CMAKE_C_FLAGS_DEBUG "-O0 -ggdb" )
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=gnu++0x -O2" )
set( CMAKE_CXX_FLAGS_DEBUG "-O0 -ggdb" )

IF ( TOOLCHAIN_NAME )
    set( IXWSS_SDK_OUTPUT_DIR "${CMAKE_BINARY_DIR}/ixwss_sdk" )
    set( CMAKE_INSTALL_PREFIX "${IXWSS_SDK_OUTPUT_DIR}" )
ELSE ( TOOLCHAIN_NAME )
    set(IXWSS_SDK_OUTPUT_DIR "${CMAKE_INSTALL_PREFIX}")
ENDIF ( TOOLCHAIN_NAME )
message (STATUS "CMAKE_INSTALL_PREFIX='${CMAKE_INSTALL_PREFIX}'") 


set( CMAKE_VERBOSE_MAKEFILE true)
#=====================================================================================
# Define directory path for sdk packaging
#=====================================================================================
set(IXWSS_LIBSRC_DIR ${CMAKE_SOURCE_DIR}/ixlib)
set(IXWSS_PLUGINS_DIR ${CMAKE_SOURCE_DIR}/ixplugins)
set(IXWSS_SAMPLES_DIR ${CMAKE_SOURCE_DIR}/samples)
set(IXWSS_CONFIG_DIR ${CMAKE_SOURCE_DIR}/ixconf)
set(IXWSS_CERT_DIR ${CMAKE_SOURCE_DIR}/ixcert)

#=====================================================================================
# Define the path of open source packages
#=====================================================================================
IF ( TOOLCHAIN_NAME )
    set( IXWSS_OPENLIBS_LIBWEBSOCKETS_LIB_DIR ${CMAKE_SOURCE_DIR}/packages/openlibs/${TOOLCHAIN_NAME}/lib )
    set( IXWSS_OPENLIBS_LIBWEBSOCKETS_HEADER_DIR ${CMAKE_SOURCE_DIR}/packages/openlibs/${TOOLCHAIN_NAME}/include )
    set( IXWSS_OPENLIBS_LIBXML2_LIB_DIR ${CMAKE_SOURCE_DIR}/packages/openlibs/${TOOLCHAIN_NAME}/lib )
    set( IXWSS_OPENLIBS_LIBXML2_HEADER_DIR ${CMAKE_SOURCE_DIR}/packages/openlibs/${TOOLCHAIN_NAME}/include/libxml2 )
    set( IXWSS_OPENLIBS_JANSSON_LIB_DIR ${CMAKE_SOURCE_DIR}/packages/openlibs/${TOOLCHAIN_NAME}/lib )
    set( IXWSS_OPENLIBS_JANSSON_HEADER_DIR ${CMAKE_SOURCE_DIR}/packages/openlibs/${TOOLCHAIN_NAME}/include )
ELSE(TOOLCHAIN_NAME)
    FIND_PACKAGE(PkgConfig REQUIRED)
    
    PKG_CHECK_MODULES( LIBWEBSOCKETS REQUIRED libwebsockets )
    set( IXWSS_OPENLIBS_LIBWEBSOCKETS_LIB_DIR ${LIBWEBSOCKETS_LIBRARY_DIRS} )
    set( IXWSS_OPENLIBS_LIBWEBSOCKETS_HEADER_DIR ${LIBWEBSOCKETS_INCLUDE_DIRS} )

    PKG_CHECK_MODULES( LIBXML2 REQUIRED libxml-2.0 )
    set( IXWSS_OPENLIBS_LIBXML2_LIB_DIR ${LIBXML2_LIBRARY_DIRS} )
    set( IXWSS_OPENLIBS_LIBXML2_HEADER_DIR ${LIBXML2_INCLUDE_DIRS} )

    PKG_CHECK_MODULES( JANSSON REQUIRED jansson )
    set( IXWSS_OPENLIBS_JANSSON_LIB_DIR ${JANSSON_LIBRARY_DIRS} )
    set( IXWSS_OPENLIBS_JANSSON_HEADER_DIR ${JANSSON_INCLUDE_DIRS} )
ENDIF(TOOLCHAIN_NAME) 

#=====================================================================================
# Sub directories
#=====================================================================================
add_subdirectory(${CMAKE_SOURCE_DIR}/ixlib)
add_subdirectory(${CMAKE_SOURCE_DIR}/ixplugins)
add_subdirectory(${CMAKE_SOURCE_DIR}/samples)
add_subdirectory(${CMAKE_SOURCE_DIR}/ixconf)
add_subdirectory(${CMAKE_SOURCE_DIR}/ixcert)

#=====================================================================================
# build sources
#=====================================================================================
#version information generation

# install readme.txt
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/readme.txt DESTINATION ${CMAKE_BINARY_DIR})

# install releasenote.txt
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/releasenote.txt DESTINATION ${CMAKE_BINARY_DIR})

add_custom_target(
    version.txt ALL
    COMMENT "Version Information Generated"
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMAND echo ${IXWSS_SDK_VERSION} > ${CMAKE_BINARY_DIR}/version.txt
)

IF( NOT DEFINED TOOLCHAIN_NAME )
    configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/ixwss.pc.in
        ${CMAKE_BINARY_DIR}/ixwss.pc
        @ONLY
    )
ENDIF( NOT DEFINED TOOLCHAIN_NAME )

#=====================================================================================            
# Install                                                                                         
#=====================================================================================            
# install readme.txt
# install releasenote.txt
# install version.txt
install (
    FILES ${CMAKE_BINARY_DIR}/readme.txt 
          ${CMAKE_BINARY_DIR}/releasenote.txt
          ${CMAKE_BINARY_DIR}/version.txt
    DESTINATION ${IXWSS_SDK_OUTPUT_DIR}
    PERMISSIONS OWNER_WRITE OWNER_READ 
                GROUP_WRITE GROUP_READ 
                WORLD_WRITE WORLD_READ ) 

IF( NOT DEFINED TOOLCHAIN_NAME )
    install (
        FILES ${CMAKE_BINARY_DIR}/ixwss.pc
        DESTINATION ${IXWSS_SDK_OUTPUT_DIR}
        PERMISSIONS OWNER_WRITE OWNER_READ 
                    GROUP_WRITE GROUP_READ 
                    WORLD_WRITE WORLD_READ ) 
ENDIF( NOT DEFINED TOOLCHAIN_NAME )

# install packages 
file( GLOB OPEN_LIBS "${CMAKE_SOURCE_DIR}/packages/openlibs/${TOOLCHAIN_NAME}/lib/*" )
install (
    FILES ${OPEN_LIBS}
    DESTINATION ${IXWSS_SDK_OUTPUT_DIR}/packages/openlibs
    PERMISSIONS OWNER_WRITE OWNER_READ 
                GROUP_WRITE GROUP_READ 
                WORLD_WRITE WORLD_READ ) 
