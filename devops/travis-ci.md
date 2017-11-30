采用gtest对cpp代码做单元测试，cmake构建工程  
一开始的做法是下载gtest的包进行编译，完成后将头文件和库文件拷贝到系统目录
在本地构建工程和编译运行都没问题，但是travis-ci端就出现问题。构建的工程出现了莫名其妙的路径和文件，以至于报错。  

后来升级了gcc和cmake的版本，但是依然会有这种问题。猜想可能是cmake构建工程时link错了库导致
[  1%] Building CXX object CMakeFiles/LeetCode-CXX.dir/cerbero/sources/gst-plugins-bad-1.0/tests/examples/gl/generic/cubeyuv/main.cpp.o
/cerbero/sources/gst-plugins-bad-1.0/tests/examples/gl/generic/cubeyuv/main.cpp:21:21: fatal error: gst/gst.h: No such file or directory
compilation terminated.
CMakeFiles/LeetCode-CXX.dir/build.make:62: recipe for target 'CMakeFiles/LeetCode-CXX.dir/cerbero/sources/gst-plugins-bad-1.0/tests/examples/gl/generic/cubeyuv/main.cpp.o' failed
make[2]: *** [CMakeFiles/LeetCode-CXX.dir/cerbero/sources/gst-plugins-bad-1.0/tests/examples/gl/generic/cubeyuv/main.cpp.o] Error 1
CMakeFiles/Makefile2:67: recipe for target 'CMakeFiles/LeetCode-CXX.dir/all' failed
make[1]: *** [CMakeFiles/LeetCode-CXX.dir/all] Error 2
Makefile:83: recipe for target 'all' failed
make: *** [all] Error 2

原因：最终匹配到的src_file 错了，貌似这边用GLOB_RECURSE出错了，把整个磁盘下符合的cpp都加入到了工程
FILE( GLOB_RECURSE SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/../../src/cpp
有可能是后面的路径不对，然后循环找了所有的文件，所引起的。