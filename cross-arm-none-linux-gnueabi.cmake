#
# CMake Toolchain file for crosscompiling on ARM.
#
# This can be used when running cmake in the following way:
#  mkdir out
#  cd out/
#  cmake ../ -DCMAKE_TOOLCHAIN_FILE=../cross-arm-none-linux-gnueabi.cmake
#
set(CMAKE_SYSTEM_NAME Linux)

if (NOT BUILD_YOCTO_RTK)
set(TOOLCHAIN_NAME arm-none-linux-gnueabi)
set(CROSS_PATH /opt/toolchains/arm-none-linux-gnueabi)
set(CMAKE_C_COMPILER "${CROSS_PATH}/bin/arm-none-linux-gnueabi-gcc")
set(CMAKE_CXX_COMPILER "${CROSS_PATH}/bin/arm-none-linux-gnueabi-g++")
set(CMAKE_LINKER "${CROSS_PATH}/bin/arm-none-linux-gnueabi-ld")
set(CMAKE_C_FLAGS ${CMAKE_CXX_FLAGS})
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
set(CMAKE_FIND_ROOT_PATH "${CROSS_PATH}")
else()
set(TOOLCHAIN_NAME rdk-glibc-x86_64-arm-toolchain-2.0)
set(CMAKE_C_COMPILER ${CC})
set(CMAKE_CXX_COMPILER ${CXX})
set(CMAKE_LINKER ${LD})
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DBUILD_YOCTO_RTK=1")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBUILD_YOCTO_RTK=1")
endif()
