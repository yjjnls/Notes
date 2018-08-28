## asm.js 的简介
C / C++ 编译成 JS 有两个最大的困难。

1.  C / C++ 是静态类型语言，而 JS 是动态类型语言。
2.  C / C++ 是手动内存管理，而 JS 依靠垃圾回收机制。
asm.js 就是为了解决这两个问题而设计的：它的变量一律都是静态类型，并且取消垃圾回收机制。除了这两点，它与 JavaScript 并无差异，也就是说，asm.js 是 JavaScript 的一个严格的子集，只能使用后者的一部分语法。

一旦 JavaScript 引擎发现运行的是 asm.js，就知道这是经过优化的代码，可以跳过语法分析这一步，直接转成汇编语言。另外，浏览器还会调用 WebGL 通过 GPU 执行 asm.js，即 asm.js 的执行引擎与普通的 JavaScript 脚本不同。这些都是 asm.js 运行较快的原因。据称，asm.js 在浏览器里的运行速度，大约是原生代码的50%左右。

### 静态类型的变量
asm.js 只提供两种[数据类型](http://asmjs.org/spec/latest/#value-types)。

*   32位带符号整数
*   64位带符号浮点数

![](http://asmjs.org/spec/latest/subtypes.png)