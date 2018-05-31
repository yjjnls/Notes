# Policies/Binary Compatibility Issues With C++

<https://community.kde.org/Policies/Binary_Compatibility_Issues_With_C%2B%2B>

所谓“二进制兼容性”指的就是在升级（也可能是 bug fix）库文件的时候，不必重新编译使用这个库的可执行文件或使用这个库的其他库文件，程序的功能不被破坏。

在解释这个定义之前，先看看 Unix/C 语言的一个历史问题：open() 的 flags 参数的取值。open(2) 函数的原型是

int open(const char \*pathname, int flags);

其中 flags 的取值有三个： O_RDONLY,  O_WRONLY,  O_RDWR。

与一般人的直觉相反，这几个值不是按位或 (bitwise-OR) 的关系，即 O_RDONLY | O_WRONLY != O_RDWR。如果你想以读写方式打开文件，必须用 O_RDWR，而不能用 (O_RDONLY | O_WRONLY)。为什么？因为 O_RDONLY, O_WRONLY, O_RDWR 的值分别是 0, 1, 2。它们不满足按位或 。

那么为什么 C 语言从诞生到现在一直没有纠正这个不足之处？比方说把 O_RDONLY, O_WRONLY, O_RDWR 分别定义为 1, 2, 3，这样 O_RDONLY | O_WRONLY == O_RDWR，符合直觉。而且这三个值都是宏定义，也不需要修改现有的源代码，只需要改改系统的头文件就行了。

**因为这么做会破坏二进制兼容性。对于已经编译好的可执行文件，它调用 open(2) 的参数是写死的，更改头文件并不能影响已经编译好的可执行文件。**比方说这个可执行文件会调用 open(path, 1) 来写 文件，而在新规定中，这表示读 文件，程序就错乱了。

## C++ ABI 的主要内容

-   函数参数传递的方式，比如 x86-64 用寄存器来传函数的前 4 个整数参数
-   `虚函数的调用`方式，通常是 vptr/vtbl 然后用 vtbl[offset] 来调用
-   struct 和 class 的内存布局，通过`偏移量`来访问数据成员
-   name mangling
-   RTTI 和异常处理的实现（以下本文不考虑异常处理）
    C/C++ 通过头文件暴露出动态库的使用方法，这个“使用方法”主要是给编译器看的，编译器会据此生成二进制代码，然后在运行的时候通过装载器(loader)把可执行文件和动态库绑到一起。**如何判断一个改动是不是二进制兼容，主要就是看头文件暴露的这份“使用说明”能否与新版本的动态库的实际使用方法兼容。因为新的库必然有新的头文件，但是现有的二进制可执行文件还是按旧的头文件来调用动态库。**

## 有哪些情况会破坏库的 ABI

这里举一些源代码兼容但是二进制代码不兼容例子

-   `给函数增加默认参数`，现有的可执行文件无法传这个额外的参数。
-   `增加虚函数`，会造成 vtbl 里的排列变化。（不要考虑“只在末尾增加”这种取巧行为，因为你的 class 可能已被继承。）
-   增加默认模板类型参数，比方说 Foo 改为 Foo >，这会改变 name mangling
-   改变 enum 的值，把 enum Color { Red = 3 }; 改为 Red = 4。这会造成错位。当然，由于 enum 自动排列取值，添加 enum 项也是不安全的，除非是在末尾添加。
-   给 class Bar 增加数据成员，造成 sizeof(Bar) 变大，以及内部数据成员的 offset 变化，这是不是安全的？通常不是安全的，但也有例外。

-   如果客户代码里有 new Bar，那么肯定不安全，因为 new 的字节数不够装下新 Bar。相反，**如果 library 通过 factory 返回 Bar\* （并通过 factory 来销毁对象）或者直接返回 shared_ptr，客户端不需要用到 sizeof(Bar)，那么可能是安全的。** 同样的道理，直接定义 Bar bar; 对象（无论是函数局部对象还是作为其他 class 的成员）也有二进制兼容问题。
-   如果客户代码里有 Bar\* pBar; pBar->memberA = xx;，那么肯定不安全，**因为 memberA 的新 Bar 的偏移可能会变**。相反，如果只通过成员函数来访问对象的数据成员，客户端不需要用到 data member 的 offsets，那么可能是安全的。
-   **如果客户调用 pBar->setMemberA(xx); 而 Bar::setMemberA() 是个 `inline function`，那么肯定不安全，因为偏移量已经被 inline 到客户的二进制代码里了。如果 setMemberA() 是 `outline function`，其实现位于 shared library 中，会随着 Bar 的更新而更新，那么可能是安全的。**
-   那么只使用 header-only 的库文件是不是安全呢？不一定。如果你的程序用了 boost 1.36.0，而你依赖的某个 library 在编译的时候用的是 1.33.1，那么你的程序和这个 library 就不能正常工作。因为 1.36.0 和 1.33.1 的 boost::function 的模板参数类型的个数不一样，其中一个多了 allocator。

## 哪些做法多半是安全的

前面我说“不能轻易修改”，暗示有些改动多半是安全的，这里有一份白名单，欢迎添加更多内容。

只要库改动不影响现有的可执行文件的二进制代码的正确性，那么就是安全的，我们可以先部署新的库，让现有的二进制程序受益。

-   增加新的 class
-   增加 non-virtual 成员函数
-   修改数据成员的名称，因为生产的二进制代码是按偏移量来访问的，当然，这会造成源码级的不兼容。
    还有很多，不一一列举了。

## 解决办法

### 采用静态链接

这个是王道。在分布式系统这，采用静态链接也带来部署上的好处，只要把可执行文件放到机器上就行运行，不用考虑它依赖的 libraries。目前 muduo 就是采用静态链接。

### 通过动态库的版本管理来控制兼容性

这需要非常小心检查每次改动的二进制兼容性并做好发布计划，比如 1.0.x 系列做到二进制兼容，1.1.x 系列做到二进制兼容，而 1.0.x 和 1.1.x 二进制不兼容。《程序员的自我修养》里边讲过 .so 文件的命名与二进制兼容性相关的话题，值得一读。

### 用 pimpl 技法，编译器防火墙

在头文件中只暴露 non-virtual 接口，并且 class 的大小固定为 sizeof(Impl\*)，这样可以随意更新库文件而不影响可执行文件。当然，这么做有多了一道间接性，可能有一定的性能损失。见 Exceptional C++ 有关条款和 C++ Coding Standards 101.

Impl为单独的一个类或者c文件，外部直接调用Impl.dll，而在Impl内部则调用含有虚函数等具体的实现类。变更时修改了具体的实现类，更新具体的实现类dll即可，而Impl.dll不变，或者也可以新增变更，但是是二进制兼容的，所以原来的可执行文件可以不改。


## 符号问题
node调用linux动态库.so，动态库再调用openssl的动态库，openssl中的符号与node相同导致冲突，程序调用了node中的地址，出现segment fault。

首先是查core文件，用gdb调试，gdb --core=core --args node xxx   
最后的堆栈中显示OBJ_NAME_add、lh_insert这两处导致最后奔溃，但定位不到文件。再在openssl中加打印，发现时ossl_init_ssl_base中出现了崩溃，EVP_add_cipher这个函数的地址是6位的（0x9130c0），而其他函数地址是12位的（0x7fc6eaf2d2a0），且EVP_add_cipher根本没有进入，而好的测试例子，其中EVP_add_cipher地址是12位的。  
怀疑是前面内存出错，导致这里函数地址出现偏差。用valgrind查内存，开头出现这样的提示
```log
==00:00:00:21.862 33389== Invalid read of size 1
==00:00:00:21.862 33389==    at 0x5DA3570: __strcmp_sse2_unaligned (strcmp-sse2-unaligned.S:24)
==00:00:00:21.862 33389==    by 0x91C969: lh_insert (in /usr/local/bin/node)
==00:00:00:21.862 33389==    by 0x929980: OBJ_NAME_add (in /usr/local/bin/node)
==00:00:00:21.862 33389==    by 0x21444964: ossl_init_ssl_base (ssl_init.c:75)
==00:00:00:21.862 33389==    by 0x21444A68: ossl_init_ssl_base_ossl_ (ssl_init.c:25)
==00:00:00:21.862 33389==    by 0x5AF5A98: __pthread_once_slow (pthread_once.c:116)
==00:00:00:21.862 33389==    by 0x21845238: CRYPTO_THREAD_run_once (threads_pthread.c:106)
==00:00:00:21.862 33389==    by 0x21444BA3: OPENSSL_init_ssl (ssl_init.c:209)
==00:00:00:21.862 33389==    by 0x2120B0EF: _gst_dtls_init_openssl (gstdtlsagent.c:128)
==00:00:00:21.862 33389==    by 0x2120DE0D: gst_dtls_certificate_class_init (gstdtlscertificate.c:106)
==00:00:00:21.862 33389==    by 0x2120DE0D: gst_dtls_certificate_class_intern_init (gstdtlscertificate.c:53)
==00:00:00:21.862 33389==    by 0xAE4921C: type_class_init_Wm (gtype.c:2232)
==00:00:00:21.862 33389==    by 0xAE4921C: g_type_class_ref (gtype.c:2947)
==00:00:00:21.862 33389==    by 0xAE2F868: g_object_new_with_properties (gobject.c:1935)
==00:00:00:21.862 33389==  Address 0x0 is not stack'd, malloc'd or (recently) free'd
```
发现OBJ_NAME_add和lh_insert是node中符号。于是把SSL_library_init放在程序起始地方，直接进行初始化，也是报错。怀疑是node-plugin提供的环境有问题，于是在node-plugin的测试例子中添加SSL_library_init，发现也会崩溃，可以确定是node环境所导致的。

node-plugin加载动态库时没有用RTLD_DEEPBIND模式，RTLD_DEEPBIND：在搜索全局符号前先搜索库内的符号，避免同名符号的冲突。导致程序在寻找EVP_add_cipher这个函数符号时，先从全局找到了node中的同名符号。且用nm命令查找node中的EVP_add_cipher符号地址，发现就是0x9130c0。

现在来看就是简单地动态库加载模式不对，但是当时在一个大系统里面，一下子来一个segment fault还是蛮棘手的，因为在上层应用中是gst_parse_launch这里出现了崩溃，所以一开始怀疑是launch的语法哪里出错了，导致了崩溃，且要和webrtc的例子一步步对比。后面还在gstreamer的插件中加打印，一步步定位出哪里出错，直到最后才想出来肯能是node环境导致的出错。