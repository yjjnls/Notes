## segment fault
内存访问越界  

进程意外退出会在当前目录下产生‘core’文件或形如‘core.数字’的文件比如‘core.1234’  
使用命令   
gdb 运行程序名 core或core.数字  
进入gdb然后使用bt命令  
可以查看进程意外退出前函数调用的堆栈，内容为从上到下列出对应从里层到外层的函数调用历史。  
如果进程意外退出不产生core文件，参考“ulimit -c core文件最大块大小”命令  

## GLib-ERROR **: Creating pipes for GWakeup: Too many open files
进程创建的线程个数超过Ubuntu 限制的每个进程可创建线程个数导致
ulimit -a 可以看到分配的文件描述符 栈内存大小等。一种方法就是修改这些环境变量

ulimit -n 4096

you should check if your code opens lots of files/filedescriptors but doesn't close them properly

用lsof查看进程打开的文件描述符
lsof |grep -i xxx|wc -l


## gdb
### start  
gdb --args WebStreamer --conf-file=webstreamer.conf

(gbd) show args
(gdb) show path
(gdb) show env xxx
(gdb) set env xxx=yyy

不打印gdb版本信息
gdb -q --args WebStreamer --conf-file=webstreamer.conf

现在只是启动了gdb，加载了环境  
run 启动程序  
start 程序停在main函数第一句  

### 调试已经在运行的程序  
attach pid：
　　从ps获得进程号，通过attach命令连接到该进程。attach一个进程后，gdb首先stop该进程，这样就可以设置断点，执行step、continue等命令；如果执行r命令，会杀掉原来的进程。

detach：
　　释放该进程，原进程会继续执行。

### break point  
break main.cpp:169  

info breakpoints [id]  

disbale/enbale id 使断点失效或者有效，但是断点仍然存在  

delete id 删除断点  

print 变量  
whatis 变量 查看类型  

next 单步  
continue

### restart  
程序结束后重新run

### core file  

#### 产生 core dump的原因
1. 内存访问越界
2. 多线程程序使用了线程不安全的函数
3. 多线程读写的数据未加锁保护
4. 堆栈溢出.不要使用大的局部变量（因为局部变量都分配在栈上），这样容易造成堆栈溢出，破坏系统的栈和堆结构，导致出现莫名其妙的错误。

#### 设置生成core file  
出错产生core file，需要设置 ulimit -c 1000（不能为0）

#### gdb查看core file  
gdb [exec file] [core file]
如: gdb ./test test.core