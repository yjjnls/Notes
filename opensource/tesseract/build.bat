set PKG_CONFIG_LIBDIR=D:\Working\MyProject\Notes\opensource\tesseract\out\lib\pkgconfig;C:\gstreamer\1.0\x86_64\lib\pkgconfig

REM rd /s/q build

mkdir build
pushd build

cmake ../example -G "Visual Studio 14 2015 Win64" 
cmake --build . --target ALL_BUILD --config Release -- /p:Platform=x64  
cmake --build . --target INSTALL   --config Release -- /p:Platform=x64 
popd
