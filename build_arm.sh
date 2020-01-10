mkdir -p out
cd out
cmake ../ -DCMAKE_TOOLCHAIN_FILE=../cross-arm-none-linux-gnueabi.cmake
make
make install
