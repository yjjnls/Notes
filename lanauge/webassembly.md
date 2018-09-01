# Webassembly
[![](https://avatars1.githubusercontent.com/u/11578470?s=200&v=4)](https://github.com/WebAssembly)

WebAssembly 是除了 JavaScript 以外，另一种可以在网页中运行的编程语言，并提供了功能和性能上的优势。可以将其它语言，如C/C++等，编译成Assembly格式的代码，然后在浏览器中运行。

## 介绍
### WebAssembly 处于哪个环节？
你想要从任意一个高级语言翻译到众多汇编语言中的一种（依赖机器内部结构），其中一种方式是创建不同的翻译器来完成各种高级语言到汇编的映射。

这种翻译的效率实在太低了。为了解决这个问题，大多数编译器都会在中间多加一层。它会把高级语言翻译到一个低层，而这个低层又没有低到机器码这个层级。这就是 **中间代码**（intermediate representation，IR）。

这就是说编译器会把高级语言翻译到 IR 语言，而编译器另外的部分再把 IR 语言编译成特定目标结构的可执行代码。

![](https://upload-images.jianshu.io/upload_images/11336404-419cee8995bc923e.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)


那么在上图中，WebAssembly 在什么位置呢？实际上，你可以把它看成另一种“目标汇编语言”。

每一种目标汇编语言（x86、ARM）都依赖于特定的机器结构。当你想要把你的代码放到用户的机器上执行的时候，你并不知道目标机器结构是什么样的。

而 WebAssembly 与其他的汇编语言不一样，它不依赖于具体的物理机器。可以抽象地理解成它是概念机器的机器语言，而不是实际的物理机器的机器语言。

正因为如此，WebAssembly 指令有时也被称为虚拟指令。它比 JavaScript 代码更直接地映射到机器码，它也代表了“如何能在通用的硬件上更有效地执行代码”的一种理念。所以它并不直接映射成特定硬件的机器码。

![](https://upload-images.jianshu.io/upload_images/11336404-cfa4040458b748c7.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

Web平台有两部分组成：a)执行应用程序的虚拟机，b)Web API,例如（DOM,WebGL等）。为了让浏览器支持Assembly，虚拟机实现了加载两种语言（javascript,Assembly）的功能，javascript可以通过Web API引入WebAssembly代码，WebAssembly可以导入和异步执行javascript的模块。

### 编译到 .wasm 文件
目前对于 WebAssembly 支持情况最好的编译器工具链是 LLVM。有很多不同的前端和后端插件可以用在 LLVM 上。

假设想从 C 语言到 WebAssembly，我们就需要 clang 前端来把 C 代码变成 LLVM 中间代码。当变换成了 LLVM IR 时，说明 LLVM 已经理解了代码，它会对代码自动地做一些优化。

为了从 LLVM IR 生成 WebAssembly，还需要后端编译器。在 LLVM 的工程中有正在开发中的后端，而且应该很快就开发完成了，现在这个时间节点，暂时还看不到它是如何起作用的。

还有一个工具叫做 **Emscripten**。**它通过自己的后端先把代码转换成自己的中间代码（叫做 `asm.js`），然后再转化成 WebAssembly，实际上它背后也是使用的 LLVM**。

![](https://upload-images.jianshu.io/upload_images/11336404-800102fede3a5503.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

Emscripten 还包含了许多额外的工具和库来包容整个 C/C++ 代码库，所以它更像是一个软件开发者工具包（SDK）而不是编译器。例如系统开发者需要文件系统以对文件进行读写，Emscripten 就有一个 IndexedDB 来模拟文件系统。

![](https://upload-images.jianshu.io/upload_images/11336404-57ded56460500c07.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)


不考虑太多的这些工具链，只要知道最终生成了 .wasm 文件就可以了。后面我会介绍 .wasm 文件的结构，在这之前先一起了解一下在 JS 中如何使用它。


### .wasm 文件结构
如果你是写高级语言的开发者，并且通过编译器编译成 WebAssembly，那你不用关心 WebAssembly 模块的结构。但是了解它的结构有助于你理解一些基本问题。

这段代码是即将生成 WebAssembly 的 C 代码：
```c
int add42(int num) { 
    return num + 42; 
}
```

你可以使用 [WASM Explorer](http://mbebenita.github.io/WasmExplorer/) 来编译这个函数。

打开 .wasm 文件（假设你的编辑器支持的话），可以看到下面代码：
```
00 61 73 6D 0D 00 00 00 01 86 80 80 80 00 01 60 

01 7F 01 7F 03 82 80 80 80 00 01 00 04 84 80 80 

80 00 01 70 00 00 05 83 80 80 80 00 01 00 01 06 

81 80 80 80 00 00 07 96 80 80 80 00 02 06 6D 65 

6D 6F 72 79 02 00 09 5F 5A 35 61 64 64 34 32 69 

00 00 0A 8D 80 80 80 00 01 87 80 80 80 00 00 20 

00 41 2A 6A 0B
```
这是 **模块的“二进制”** 表示。之所以用引号把“二进制”引起来，是因为上面其实是用十六进制表示的，不过把它变成二进制或者人们能看懂的十进制表示也很容易。

例如，下面是 num + 42 的各种表示方法。

![](https://upload-images.jianshu.io/upload_images/11336404-4a9eae3541b7cc0a.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

### 代码是如何工作的：基于栈的虚拟机
如果你对具体的操作过程很好奇，那么这幅图可以告诉你指令都做了什么。

![](https://upload-images.jianshu.io/upload_images/11336404-9c5c798b2f43391f.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

从图中我们可以注意到 加 操作并没有指定哪两个数字进行加。这是因为 WebAssembly 是采用“基于栈的虚拟机”的机制。即一个操作符所需要的所有值，在操作进行之前都已经存放在堆栈中。

所有的操作符，比如加法，都知道自己需要多少个值。加需要两个值，所以它从堆栈顶部取两个值就可以了。那么加指令就可以变的更短（单字节），因为指令不需要指定源寄存器和目的寄存器。这也使得 .wasm 文件变得更小，进而使得加载 .wasm 文件更快。

尽管 WebAssembly 使用基于栈的虚拟机，但是并不是说在实际的物理机器上它就是这么生效的。当浏览器翻译 WebAssembly 到机器码时，浏览器会使用寄存器，而 WebAssembly 代码并不指定用哪些寄存器，这样做的好处是给浏览器最大的自由度，让其自己来进行寄存器的最佳分配。

### WebAssembly 模块的组成部分
除了上面介绍的，.wasm 文件还有其他部分。一些组成部分对于模块来讲是必须的，一些是可选的。

必须部分：

*   Type。在模块中定义的函数的函数声明和所有引入函数的函数声明。
*   Function。给出模块中每个函数一个索引。
*   Code。模块中每个函数的实际函数体。

可选部分：

*   Export。使函数、内存、表（tables）、全局变量等对其他 WebAssembly 或 JavaScript 可见，允许动态链接一些分开编译的组件，即 .dll 的WebAssembly 版本。
*   Import。允许从其他 WebAssembly 或者 JavaScript 中导入指定的函数、内存、表或者全局变量。
*   Start。当 WebAssembly 模块加载进来的时候，可以自动运行的函数（类似于 main 函数）。
*   Global。声明模块的全局变量。
*   Memory。定义模块用到的内存。
*   Table。使得可以映射到 WebAssembly 模块以外的值，如映射到 JavaScript 的对象。这在间接函数调用时很有用。
*   Data。初始化导入的或者局部内存。
*   Element。初始化导入的或者局部的表。

如果你想了解关于这些组成部分的更深入的内容，可以阅读这些[组成部分的工作原理](https://rsms.me/wasm-intro)。

## [安装环境](https://webassembly.org/getting-started/developers-guide/)

```sh
$ cd /opt && git clone https://github.com/juj/emsdk.git
$ cd emsdk
$ sudo ./emsdk install latest
$ sudo ./emsdk activate latest
# load environment
$ source ./emsdk_env.sh
```

简单测试用例

```sh
$ mkdir hello
$ cd hello
$ cat << EOF > hello.c
#include <stdio.h>
int main(int argc, char ** argv) {
  printf("Hello, world!\n");
}
EOF
$ emcc hello.c -s WASM=1 -o hello.html

$ emrun --no_browser --port 8080 .
```

然后在你的[浏览器中打开](http://localhost:8080/)，你就会在Emscripten的控制台上看到“Hello, world!”，说明你已经成功安装了webassembly环境。

## 高级工具
WebAssembly由许多工具支持，以帮助开发人员构建和处理源文件并生成二进制内容。目前有两个高级工具：

*   [WABT](https://github.com/WebAssembly/wabt) - WebAssembly二进制工具包
*   [Binaryen](https://github.com/WebAssembly/binaryen) - 编译器和工具链基础结构

### WABT：WebAssembly二进制工具包
此工具包支持将二进制WebAssembly格式转换为人类可读文本格式和从人类可读文本格式转换。文本格式是一种S表达式，可以方便地使用WebAssembly编译器的输出进行分析或调试等。

请注意，WABT支持的[S表达式](https://en.wikipedia.org/wiki/S-expression)格式不是WebAssembly本身。它是可以表示WebAssembly文件内容的许多可能的文本格式之一，因此它被开发为WABT工具解码和编码的便捷格式。开发人员可以轻松地为能够表达WebAssembly堆栈机器语义的任何其他文本格式构建解码器/编码器实现。

*   wasm2wat  
    此工具将WebAssembly二进制文件转换为S表达式。它是一个命令行工具，它将二进制文件作为输入，并生成包含可读文本的输出文件。开发人员可以以其他方式编辑或操作文本文件，并将其转换回二进制格式，例如尝试优化算法，跟踪，插入调试钩子等。
*   wat2wasm  
    此命令行工具执行wasm2wat的反转，即它将S表达式wat文件转换为二进制WebAssembly文件。使用wasm2wat和wat2wasm可以实现WebAssembly二进制文件的无损往返，并为开发人员提供了一种使用外部工具操作WebAssembly二进制文件内容的便捷方法。
*   wasm-interp  
    这是一个解释器，**允许开发人员从命令行独立运行WebAssembly二进制文件**。它实现了一个 **基于堆栈机器的解释器**，可以直接解释WebAssembly二进制文件。这与浏览器在加载时将WebAssembly二进制文件JIT作为其目标体系结构的本机代码的方式不同。**解释器可用于在浏览器环境之外运行单元测试，验证WebAssembly二进制文件等**。
*   wasm-objdump  
    打印有关wasm二进制文件的信息，与objdump类似。
*   wat-desugar  
    解析.wat文本格式，由spec解释器支持（s表达式，平面语法或混合）并打印“规范”平面格式。
*   wasm2c  
    将WebAssembly二进制文件转换为C源文件和头文件。


### Binaryen
Binaryen是一套全面的工具，具有支持基础结构，可用作将WebAssembly作为输出格式的编译器的后端。它有一个C API并实现了自己的程序逻辑内部中间表示（IR），可以在IR上执行许多优化，支持代码生成的并行化等。

例如，binaryen用作编译器asm2wasm的一部分，可以将asm.js文件转换为WebAssembly文件。它还用于支持WebAssembly 的LLVM编译器基础结构生成和Rust的编译。

从事编译器，高级优化技术等工作的开发人员应该利用binaryen及其工具，其中包括可以加载和解释WebAssembly代码，汇编器和反汇编器的shell，asm.js的转换器和LLVM .s文件到WebAssembly等等。

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
两者的功能基本一致，就是转出来的代码不一样：**asm.js 是文本，WebAssembly 是二进制字节码**，因此运行速度更快、体积更小。从长远来看，WebAssembly 的前景更光明。

但是，这并不意味着 asm.js 肯定会被淘汰，因为它有两个优点：首先，它是文本，人类可读，比较直观；其次，所有浏览器都支持 asm.js，不会有兼容性问题。

---

# Emscripten
[![](http://kripken.github.io/emscripten-site//_static/Emscripten_logo_full.png)](http://kripken.github.io/emscripten-site/index.html)

## [语法](http://kripken.github.io/emscripten-site/docs/tools_reference/emcc.html)
    emcc [options] file...

输入文件可以是c/c++源代码，LLVM二进制字节码或者是LLVM汇编文件。    
参数：
*   -O 优化级别。
*   -s OPTION=VALUE 传给Emscripten编译器的参数，具体可以参考[设置文件](https://github.com/kripken/emscripten/blob/master/src/settings.js)。
*   -g 保存调试信息。如果编译成字节码，那么和clang和gcc的参数意义一样；如果是从字节码或者源文件编译成javascript，那么等同于-g3。
*   -g\<level> 不同调试级别。
*   --pre-js \<file> 指定的文件会在生成代码和优化前加入到生成文件中去。
*   --embed-file \<file> 指定文件嵌入生成的js中，适用于小文件。
*   --preload-file \<name>  在运行代码之前先异步地加载指定文件。
*   -o \<target> 指定生成的目标文件
    \<name> .js : JavaScript.
    \<name> .html : HTML + separate JavaScript file (<name>.js). Having the separate JavaScript file improves page load time.
    \<name> .bc : LLVM bitcode (default).
    \<name> .o : LLVM bitcode (same as .bc).

举个例子
```
emcc hello.c -s WASM=1 -O2 -o hello.html
```
* -s WASM=1  指定使用WebAssembly，WASM=0表示生成asm.js
* -O2 代码优化级别
* -o 指定生成文件，可以是html、js或字节码.bc

一般会生成.wasm(webassembly code)、 .js(glue code) 以及html。胶接代码glue code可以让js调用webassembly。

## 构建
### single file
```sh
# Generate a.out.js from C++. Can also take .ll (LLVM assembly) or .bc (LLVM bitcode) as input
./emcc src.cpp

# Generate src.o containing LLVM bitcode.
./emcc src.cpp -c

# Generate result.js containing JavaScript.
./emcc src.cpp -o result.js

# Generate result.bc containing LLVM bitcode (the suffix matters).
./emcc src.cpp -o result.bc

# Generate a.out.js from two C++ sources.
./emcc src1.cpp src2.cpp

# Generate src1.o and src2.o, containing LLVM bitcode
./emcc src1.cpp src2.cpp -c

# Combine two LLVM bitcode files into a.out.js
./emcc src1.o src2.o

# Combine two LLVM bitcode files into another LLVM bitcode file
./emcc src1.o src2.o -o combined.o
```
### [project](http://kripken.github.io/emscripten-site/docs/compiling/Building-Projects.html)
```sh
# Compile libstuff to bitcode
./emconfigure ./configure
./emmake make

# Compile project to bitcode
./emconfigure ./configure
./emmake make

# Compile the library and code together to HTML
emcc project.bc libstuff.bc -o final.html

####################################################
# Generate bitcode files project.bc and libstuff.bc
...

# Link together the bitcode files
emcc project.bc libstuff.bc -o allproject.bc

# Compile the combined bitcode to HTML
emcc allproject.bc -o final.html
```


## C++与JavaScript交互
### JS调用webassembly
一种方法是通过ccall来调用，该方法允许我们通过函数名从C代码中调用一个函数，然后就向一般的JS函数一样使用就行了。
```js
var result = Module.ccall(
    'funcName',     // 函数名
    'number',       // 返回类型
    ['number'],     // 参数类型
    [42]);          // 参数
```
调用此方法之后，result就将拥有对应C函数的所有功能，除函数名以外的所有参数都是可选的。

通过在函数名前添加下划线来调用C函数
var result = _funcName();

```c
//api_example.c
#include <stdio.h>
#include <emscripten.h>

EMSCRIPTEN_KEEPALIVE
void sayHi() {
  printf("Hi!\n");
}

EMSCRIPTEN_KEEPALIVE
int daysInWeek() {
  return 7;
}
```
```sh
$ emcc api_example.c -o api_example.js
```
```js
var em_module = require('./api_example.js');

em_module._sayHi(); // direct calling works
em_module.ccall("sayHi"); // using ccall etc. also work
console.log(em_module._daysInWeek()); // values can be returned, etc.
```

ref [How to Use WebAssembly with Node.js](https://www.codepool.biz/use-webassembly-node-js.html)


## 文件系统
本机代码和“普通”JavaScript使用完全不同的文件访问方式。可移植的本机代码通常在libc和libcxx中调用同步文件API，依次调用底层文件系统API，默认情况下使用MEMFS虚拟文件系统。而JavaScript只允许异步文件访问（Web worker除外），此外，在Web浏览器提供的沙箱环境中运行时，JavaScript无法直接访问主机文件系统。

![](http://kripken.github.io/emscripten-site/_images/FileSystemArchitecture.png)

Emscripten提供了一个模拟本地文件系统的虚拟文件系统，因此可以编译和运行使用同步文件API的本机代码，只需很少或不需要更改。

在运行时库初始化时，MEMFS会挂载在/目录。要添加到MEMFS虚拟文件系统的文件是在编译时使用emcc指定的，如Packaging Files中所述。首次加载页面时，使用Synchronous XHR通过JavaScript异步加载文件。只有在异步下载完成且文件在虚拟文件系统中可用时，才允许编译的代码运行（并调用同步API）。

随着MEMFS所有文件严格存在于内存中，当重新加载页面写入到其中的任何数据都将丢失。如果需要持久化数据可以装入IDBFS在浏览器或文件系统NODEFS上的node.js。NODEFS提供对本地文件系统的直接访问，但仅限于在node.js内运行时。您可以直接从自己的JavaScript 调用文件系统API来安装新的文件系统，并执行可能需要的其他同步文件系统操作。在[文件系统](http://kripken.github.io/emscripten-site/docs/api_reference/Filesystem-API.html#filesystem-api-filesystems)中有关于此主题的更多信息。

要使用Emscripten提供的虚拟文件系统，需要将文件[打包](http://kripken.github.io/emscripten-site/docs/porting/files/packaging_files.html)。  
文件的打包方式有两种：`预加载`和`嵌入`。嵌入将指定的文件放在生成的JavaScript中，而预加载则单独打包文件。嵌入文件的效率远低于预加载，只应在包装少量小文件时使用。预加载还允许选项单独托管数据。 

打包的默认方法是在编译时将嵌套文件结构（相对于编译时命令提示符目录）直接映射到虚拟文件系统的根目录。若想改变文件在虚拟文件系统中的位置，可以在构建时用@符号指定资源在运行时的路径。

例如，我们可以使用以下方法将预加载的文件夹../../asset_dir映射到虚拟文件系统的根目录（/）：

    ./emcc file.cpp -o file.html --preload-file ../../asset_dir@/

我们还可以映射新的路径和文件名。例如，要使嵌入文件../res/gen123.png可用作/main.png，我们可能会这样做：

    ./emcc file.cpp -o file.html --embed-file ../res/gen123.png@main.png

下面的字符可能在文件名中使用`A-Z`，`a-z`，`0-9`，空格字符和任意字符!#$%&'()+,-.;=@[]^_`{}~。此外，如果您的主机文件系统支持以下字符，则可以使用以下字符:（"*<>?|Windows不允许在文件名中使用这些字符）。在@命令行上指定字符时，必须将其转义为表单@@以避免触发src@dst映射表示法（参见上文）。字符/，\并且:不能使用。

预加载可以支持多种文件，比如图片（.jpg, .jpeg, .png, .bmp）、音频（.ogg, .wav, .mp3）还有动态库（.so）。将动态库加载到虚拟文件系统中后，可以用dlopen打开；另一种方法是直接编译成字节码，和c++源码一起编译到目标文件中去。

参考 [File System API](http://kripken.github.io/emscripten-site/docs/api_reference/Filesystem-API.html#filesystem-api)     
[Asynchronous File System API](http://kripken.github.io/emscripten-site/docs/api_reference/emscripten.h.html#emscripten-h-asynchronous-file-system-api)

## 异步化
C++中的sleep调用是一个阻塞的过程，如果直接编译到js，那么会阻塞js的循环，这是不可取的。应该将其翻译为js中的异步sleep函数，但为了减少c++代码的改动，可以用[emscripten_sleep()](http://kripken.github.io/emscripten-site/docs/porting/asyncify.html)函数来代替sleep()，并在编译时加上`-s ASYNCIFY=1`参数。

还有些[方法](http://kripken.github.io/emscripten-site/docs/porting/asyncify.html#extensions)可以将c中的同步函数改造成JS中的异步函数。

async改造会拖慢程序，setjmp/longjmp和c++异常也不适用async改造。