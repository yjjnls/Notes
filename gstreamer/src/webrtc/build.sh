mkdir -p build
cd build
cmake .. -DCMAKE_INSTALL_PREFIX="../tests" 
make 
make install