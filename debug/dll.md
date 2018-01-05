# 二进制兼容性
所谓“二进制兼容性”指的就是在升级（也可能是 bug fix）库文件的时候，不必重新编译使用这个库的可执行文件或使用这个库的其他库文件，程序的功能不被破坏。

```
在解释这个定义之前，先看看 Unix/C 语言的一个历史问题：open() 的 flags 参数的取值。open(2) 函数的原型是

int open(const char *pathname, int flags);

其中 flags 的取值有三个： O_RDONLY,  O_WRONLY,  O_RDWR。

与一般人的直觉相反，这几个值不是按位或 (bitwise-OR) 的关系，即 O_RDONLY | O_WRONLY != O_RDWR。如果你想以读写方式打开文件，必须用 O_RDWR，而不能用 (O_RDONLY | O_WRONLY)。为什么？因为 O_RDONLY, O_WRONLY, O_RDWR 的值分别是 0, 1, 2。它们不满足按位或 。

那么为什么 C 语言从诞生到现在一直没有纠正这个不足之处？比方说把 O_RDONLY, O_WRONLY, O_RDWR 分别定义为 1, 2, 3，这样 O_RDONLY | O_WRONLY == O_RDWR，符合直觉。而且这三个值都是宏定义，也不需要修改现有的源代码，只需要改改系统的头文件就行了。

因为这么做会破坏二进制兼容性。对于已经编译好的可执行文件，它调用 open(2) 的参数是写死的，更改头文件并不能影响已经编译好的可执行文件。比方说这个可执行文件会调用 open(path, 1) 来写 文件，而在新规定中，这表示读 文件，程序就错乱了。
```