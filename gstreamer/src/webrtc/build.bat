set PATH=C:\cerbero\build-tools\bin;%PATH%
set PKG_CONFIG_LIBDIR=C:\cerbero\x86_64\lib\pkgconfig

REM rd /s/q build

mkdir build
pushd build

cmake .. -G "Visual Studio 14 2015 Win64" -DCMAKE_INSTALL_PREFIX="../tests" 
cmake --build . --target ALL_BUILD --config Debug -- /p:Platform=x64  
cmake --build . --target INSTALL   --config Debug -- /p:Platform=x64 
popd
