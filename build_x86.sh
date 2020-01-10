mkdir -p out
cd out
cmake .. -DTOOLCHAIN_NAME=x86
make
make install
