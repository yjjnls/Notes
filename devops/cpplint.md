# Cpplint
cpplint是基于[Google编程规范 c++](https://google.github.io/styleguide/cppguide.html)的一款代码规范检查工具，用于检查c/c++代码中不符合编程规范的地方，便于统一同组开发人员之间的代码风格和规范。

## 安装
cpplint的安装依赖于python，运行如下指令进行安装
```shell
$ pip install cpplint
```

## 使用
cpplint的使用方式如下

```shell
$ cpplint [OPTIONS] files
```

files为要检测的c/c++头文件或源文件。  

OPTIONS选项可以通过 `cpplint --help` 指令获取其详细参数，一般我们会适当设置一些参数来调整检测规则，以便适用于我们的编程习惯。这里常用的是`--filter`和`--linelength`两个参数，前者用于过滤一些不是十分必要的规则，后者用于调整每行字符数的检测（默认为80，比较小）。

```shell
$ cpplint --filter=-whitespace/tab,-whitespace/braces,-build/header_guard,-readability/casting,-build/include_order,-build/include --linelength=120 xxx.cc
```

如果代码中有不符合规范的地方，会出现如下结果，可以根据代码行位置和错误类型快速修改。   
![](https://github.com/yjjnls/Notes/blob/master/img/cpplint.png)