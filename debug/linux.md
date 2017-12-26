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

