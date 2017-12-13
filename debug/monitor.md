- [Process](#process)
    - [Process ID](#process-id)
        - [pidof](#pidof)
        - [grep](#grep)
            - [ignore case](#ignore-case)
            - [regular expression](#regular-expression)
    - [Process numbers](#process-numbers)
    - [Process Status](#process-status)
        - [Process informations](#process-informations)
        - [占用cpu](#%E5%8D%A0%E7%94%A8cpu)
            - [定时](#%E5%AE%9A%E6%97%B6)
        - [basics](#basics)
- [Monitor tools](#monitor-tools)
    - [htop](#htop)
    - [gprof](#gprof)
    - [valgrind](#valgrind)
        - [Callgrind](#callgrind)
        - [Memcheck](#memcheck)

# Process
## Process ID
### pidof
```
pidof xxx
```
### grep
```
ps -e|grep xxx
第一列就为pid
```
#### ignore case
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
.\* 匹配任意字符串（和shell中\*一样）  

```
grep -i 'javastrea'
-> JavaStrea[mer]

grep -i 'javastrea.'
-> JavaStream[er]

grep -i 'javastrea.*'
-> JavaStreamer
```

## Process numbers

ps -eLf|grep -i xxx|wc -l


## Process Status
### Process informations
```
top -p pid1,pid2,pid3
```

### 占用cpu
---
查看/proc/*/stat中的信息

---
    ps -aux|grep -i xxx|grep -v grep|awk '{print $3;}'  
该方法是错误的！！！！！！！！
`ps`命令所测出来的cpu是指   `一个进程占用cpu的时间/该进程运行的总时间`

----
而`top`命令表示`在一个固定的间隔时间内，某个进程使用的CPU时间占总CPU时间（即这段间隔时间）的比值。`  
默认显示为Irix模式，即为相对于单个cpu core的占用的时间，如双核机器需要将该数值除以2＊2计算，通过 shift+i可以进行切换；

    top -d 2|grep -i xxx >>top.txt

---
vmstat 的100%-idle == ps的cpu占用/cpu核心数 == top的cpu占用/cpu核心数/物理cpu数目

---
#### 定时
step1：写cron脚本文件，命名为crontest.cron。
15,30,45,59 * * * * echo "xgmtest....."     表示，每隔15分钟，执行一次打印命令 

step2：添加定时任务。执行命令  

    crontab /home/del/crontest.cron >~/log

**要用绝对路径，不会输出到屏幕，要重定向**

查看cron状态  

    sudo  service cron status　
开启cron  

    sudo /etc/init.d/cron start
关闭cron  

    sudo /etc/init.d/cron stop
重启cron  

    sudo /etc/init.d/cron restart


### basics
一般进程要经历：就绪态（获取了其他资源等待cpu），阻塞态，运行态
阻塞态：进程因为需要等待所需资源而放弃处理器（如系统调用阻塞或者sleep），或者进程本不拥有处理器，且其他资源也没有满足，从而即使得到处理器也不能开始运行。
运行态：进程得到了处理器，**并不需要等待其他任何资源**，正在执行的状态，称之为运行态。**只有在运行态时，进程才可以使用所申请到的资源**。在所有进程中同一时刻仅有一个进程处于运行态。

**linux**  
`RUNNING`：正在运行或者在就绪队列中等待运行的进程。**一个进程处于RUNNING状态，并不代表他一定在被执行。**由于在多任务系统中，各个就绪进程需要并发执行，所以在某个特定时刻，**这些处于RUNNING状态的进程之中，只有一个能得到处理器**，而其他进程必须在一个就绪队列中等待。  
`UNINTERRUPTABLE`:不可中断阻塞状态。处于这种状态的进程正在等待队列中，**当资源有效时，可由操作系统进行唤醒**，否则，将一直处于等待状态。   
`INTERRUPTABLE`：可中断阻塞状态。与不可中断阻塞状态一样，处于这种状态的进程在等待队列中，当资源有效时，可以有操作系统进行唤醒。与不可中断阻塞状态有所区别的是，**处于此状态中的进程亦可被其他进程的信号唤醒**。   
`STOPPED`：挂起状态。进程被暂停，需要通过其它进程的信号才能被唤醒。导致这种状态的原因有两种。其一是受到相关信号(SIGSTOP,SIGSTP,SIGTTIN或SIGTTOU)的反应。其二是受到父进程ptrace调用的控制，而暂时将处理器交给控制进程。   
`ZOMBIE`：僵尸状态。表示进程结束但尚未消亡的一种状态。**此时进程已经结束运行并释放掉大部分资源，但尚未释放进程控制块，进程描述符仍然存在**。 



R（Running）(TASK_RUNNING)：可执行状态&运行状态；

S（Sleep）(TASK_INTERRUPTIBLE)：该进程目前正在睡眠状态（idle），但可以被唤醒（signal）；

D (TASK_UNINTERRUPTIBLE)：不可被唤醒的状态，通常这个进程可能在等待I/O的情况（ex>打印）；

T (TASK_STOPPED or TASK_TRACED)：停止状态（stop）(TASK_STOPPED or TASK_TRACED)，可能是在工作控制（后台**暂停**）或**跟踪**（traced）状态；

Z（Zombie）(TASK_DEAD - EXIT_ZOMBIE)：“僵尸”状态，该进程已经终止但却无法被删除至内存外。不可被kill,　即不响应任务信号,　无法用SIGKILL杀死


cpu负载？D状态 Z状态？
pstack？

--------------------------


# Monitor tools
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

### Callgrind
valgrind --tool=callgrind WebStreamer --conf-file=webstreamer.conf  
会生成一个callgrind.out.28982  
python gprof2dot.py -f callgrind -n10 -s callgrind.out.28982 > valgrind.dot  
dot -Tpng valgrind.dot -o valgrind.png  

gprof2dot.py -f callgrind callgrind.out.XXX |dot -Tpng -o report.png  

http://blog.csdn.net/sunmenggmail/article/details/10543483  

### Memcheck
https://www.cnblogs.com/findumars/p/5087306.html  