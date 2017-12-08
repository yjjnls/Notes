
### 查找进程ID
### pidof
```
pidof xxx
```
### grep
```
ps -e|grep xxx
第一列就为pid
```
#### 忽略大小写
```
grep -i javastreamer
-> JavaStreamer
```
-n 显示在文件中查找到的内容所在行
-c 显示匹配次数

#### regular expression
正则表达式要用''括起来，与shell中通配符也有区别

^char 以某个字符开头  
char$ 以某个字符结尾  
. 任意一个字符  
\* 前面的字符零到多次  
.* 匹配任意字符串（和shell中*一样）  

```
grep -i 'javastrea'
-> JavaStrea[mer]

grep -i 'javastrea.'
-> JavaStream[er]

grep -i 'javastrea.*'
-> JavaStreamer
```


### 监控某一进程的信息

```
top -p pid1,pid2,pid3
```

### 查看某个进程的线程数

ps -eLf|grep -i webstreamer|wc -l


## htop
PID：进行的标识号
USER：运行此进程的用户
**PRI：进程的优先级**
**NI：进程的优先级别值，默认的为0，可以进行调整**
VIRT：进程占用的虚拟内存值
RES：进程占用的物理内存值
SHR：进程占用的共享内存值
**S：进程的运行状况，R表示正在运行、S表示休眠，等待唤醒、Z表示僵死状态**
%CPU：该进程占用的CPU使用率
%MEM：该进程占用的物理内存和总内存的百分比
TIME+：该进程启动后占用的总的CPU时间
COMMAND：进程启动的启动命令名称

## gprof
[gprof](http://blog.csdn.net/stanjiang2010/article/details/5655143)只能找找函数调用关系和函数级别的性能，它是通过在既有程序中编译时插入代码来实现的,属于Instrumentation profiler。
* 编译时加上 -pg 选项  
* 程序运行完成后会在当前目录下生成gmon.out文件  
* gprof ./tmp | gprof2dot.py |dot -Tpng -o report.png
* 或者 gprof ./tmp gmon.out >report.txt

问题：第三方库内的函数无法跟踪

## valgrind
gprof相当于valgrind加上工具集里的callgrind

Memcheck：用于检测内存错误。它帮助c和c++的程序更正确。
Cachegrind：用于分析cache和分支预测。它帮助程序执行得更快。
Callgrind：用于函数调用的分析。
Helgrind：用于分析多线程。
DRD：也用于分析多线程。与Helgrind类似，但是用不同的分析技术，所以可以检测不同的问题。
Massif：用于分析堆。它帮助程序精简内存的使用。
SGcheck：检测栈和全局数组溢出的实验性工具，它和Memcheck互补使用。


Helgrind可以检测下面三类错误：
1.POSIX pthreads API的错误使用
2.由加锁和解锁顺序引起的潜在的死锁
3.数据竞态--在没有锁或者同步机制下访问内存

### callgrind
valgrind --tool=callgrind WebStreamer --conf-file=webstreamer.conf
会生成一个callgrind.out.28982
python gprof2dot.py -f callgrind -n10 -s callgrind.out.28982 > valgrind.dot
dot -Tpng valgrind.dot -o valgrind.png