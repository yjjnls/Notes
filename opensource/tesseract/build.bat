mkdir build && pushd build
cppan ../tesseract
cmake ../tesseract -G "Visual Studio 14 2015 Win64" -DCMAKE_INSTALL_PREFIX="../out"
cmake --build . --target ALL_BUILD --config Release -- /p:Platform=x64  
cmake --build . --target INSTALL   --config Release -- /p:Platform=x64 

popd