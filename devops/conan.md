travis上面用conan docker时，先运行install和script里面的脚本。

script里面一般是`python build.py`，所以即使docker里面有conan，install里面也要先安装conan。

先运行build.py，先运行conanfile.py普通代码，然后下载进入docker，在docker里面再次运行conanfile.py