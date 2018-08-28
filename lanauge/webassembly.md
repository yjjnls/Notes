# Webassembly
[![](https://avatars1.githubusercontent.com/u/11578470?s=200&v=4)](https://github.com/WebAssembly)

WebAssembly 是除了 JavaScript 以外，另一种可以在网页中运行的编程语言。WebAssembly 模块定义的一些功能可以通过 JavaScript 来调用。

## WebAssembly 处于哪个环节？
你想要从任意一个高级语言翻译到众多汇编语言中的一种（依赖机器内部结构），其中一种方式是创建不同的翻译器来完成各种高级语言到汇编的映射。

这种翻译的效率实在太低了。为了解决这个问题，大多数编译器都会在中间多加一层。它会把高级语言翻译到一个低层，而这个低层又没有低到机器码这个层级。这就是中间代码（intermediate representation，IR）。

这就是说编译器会把高级语言翻译到 IR 语言，而编译器另外的部分再把 IR 语言编译成特定目标结构的可执行代码。

![](https://img-blog.csdn.net/20170324103635846?)



## asm.js 的简介
asm.js 不仅能让浏览器运行 3D 游戏，还可以运行各种服务器软件，比如 Lua、Ruby 和 SQLite。 这意味着很多工具和算法，都可以使用现成的代码，不用重新写一遍。

另外，由于 asm.js 的运行速度较快，所以一些计算密集型的操作（比如计算 Hash）可以使用 C / C++ 实现，再在 JS 中调用它们。

不过C / C++ 编译成 JS 有两个最大的困难。

1.  C / C++ 是静态类型语言，而 JS 是动态类型语言。
2.  C / C++ 是手动内存管理，而 JS 依靠垃圾回收机制。

asm.js 就是为了解决这两个问题而设计的：它的变量一律都是静态类型，并且取消垃圾回收机制。除了这两点，它与 JavaScript 并无差异，也就是说，asm.js 是 JavaScript 的一个严格的子集，只能使用后者的一部分语法。

一旦 JavaScript 引擎发现运行的是 asm.js，就知道这是经过优化的代码，可以跳过语法分析这一步，直接转成汇编语言。另外，浏览器还会调用 WebGL 通过 GPU 执行 asm.js，即 asm.js 的执行引擎与普通的 JavaScript 脚本不同。这些都是 asm.js 运行较快的原因。据称，asm.js 在浏览器里的运行速度，大约是原生代码的50%左右。

asm.js 只提供两种[数据类型](http://asmjs.org/spec/latest/#value-types)。

*   32位带符号整数
*   64位带符号浮点数

![](http://asmjs.org/spec/latest/subtypes.png)

其他数据类型，比如字符串、布尔值或者对象，asm.js 一概不提供。它们都是以数值的形式存在，保存在内存中，通过 TypedArray 调用。

如果变量的类型要在运行时确定，asm.js 就要求事先声明类型，并且不得改变，这样就节省了类型判断的时间。

asm.js 没有垃圾回收机制，所有内存操作都由程序员自己控制。asm.js 通过 TypedArray 直接读写内存。

### asm.js 与 WebAssembly 的异同
两者的功能基本一致，就是转出来的代码不一样：asm.js 是文本，WebAssembly 是二进制字节码，因此运行速度更快、体积更小。从长远来看，WebAssembly 的前景更光明。

但是，这并不意味着 asm.js 肯定会被淘汰，因为它有两个优点：首先，它是文本，人类可读，比较直观；其次，所有浏览器都支持 asm.js，不会有兼容性问题。

## Emscripten 编译器
![](http://kripken.github.io/emscripten-site//_static/Emscripten_logo_full.png)