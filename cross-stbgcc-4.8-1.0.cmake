#
# CMake Toolchain file for crosscompiling on ARM.
#
# This can be used when running cmake in the following way:
#  mkdir out
#  cd out/
#  cmake ../ -DCMAKE_TOOLCHAIN_FILE=../cross-stbgcc-4.8-1.0.cmake
#
set(CMAKE_SYSTEM_NAME Linux)
set(TOOLCHAIN_NAME stbgcc)

set(CROSS_PATH /opt/toolchains/cross-stbgcc-4.8-1.0)
set(CMAKE_C_COMPILER ${CC})
set(CMAKE_CXX_COMPILER ${CXX})
set(CMAKE_LINKER ${LD})
set(CMAKE_C_FLAGS ${CMAKE_CXX_FLAGS})
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
