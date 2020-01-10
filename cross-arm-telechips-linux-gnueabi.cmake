#
# CMake Toolchain file for crosscompiling on ARM.
#
# This can be used when running cmake in the following way:
#  mkdir out
#  cd out/
#  cmake ../ -DCMAKE_TOOLCHAIN_FILE=../cross-arm-none-linux-gnueabi.cmake
#

set(CMAKE_SYSTEM_NAME Linux)

set(TOOLCHAIN_NAME cortexa7-vfp-neon-telechips-linux-gnueabi)
set(CROSS_PATH /home/kskim2/works/cid/tcc/sysroots/x86_64-oesdk-linux/usr)
set(CMAKE_C_COMPILER "${CROSS_PATH}/bin/arm-telechips-linux/arm-telechips-linux-gcc")
set(CMAKE_CXX_COMPILER "${CROSS_PATH}/bin/arm-telechips-linux/arm-telechips-linux-g++")
set(CMAKE_LINKER "${CROSS_PATH}/bin/arm-telechips-linux/arm-telechips-linux-ld")

SET(SYSROOT "/home/kskim2/works/cid/tcc/sysroots/cortexa7-vfp-neon-telechips-linux-gnueabi")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --sysroot=${SYSROOT}" CACHE INTERNAL "" FORCE)
set(CMAKE_C_LINK_FLAGS "${CMAKE_C_LINK_FLAGS} --sysroot=${SYSROOT}" CACHE INTERNAL "" FORCE)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --sysroot=${SYSROOT}" CACHE INTERNAL "" FORCE)
set(CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS} --sysroot=${SYSROOT}" CACHE INTERNAL "" FORCE)
