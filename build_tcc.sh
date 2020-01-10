mkdir -p out
cd out
cmake ../ -DCMAKE_TOOLCHAIN_FILE=../cross-arm-telechips-linux-gnueabi.cmake
make
make install
