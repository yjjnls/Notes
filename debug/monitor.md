
- [Process](#process)
    - [Process ID](#process-id)
        - [pidof](#pidof)
        - [grep](#grep)
            - [ignore case](#ignore-case)
            - [regular expression](#regular-expression)
    - [Process numbers](#process-numbers)
    - [Process Status](#process-status)
        - [Process informations](#process-informations)
        - [占用cpu](#占用cpu)
            - [定时](#定时)
        - [basics](#basics)
            - [**linux状态**](#linux状态)
            - [linux进程状态标识](#linux进程状态标识)
            - [T进程处理方法](#t进程处理方法)
            - [D和Z进程处理方法](#d和z进程处理方法)
    - [Process fd](#process-fd)
- [磁盘空间](#磁盘空间)
- [Monitor tools](#monitor-tools)
    - [linux](#linux)
        - [htop](#htop)
        - [gprof](#gprof)
        - [valgrind](#valgrind)
            - [Callgrind](#callgrind)
            - [Memcheck](#memcheck)
        - [perf](#perf)
    - [windows](#windows)
        - [ProcDump](#procdump)
            - [获取异常进程的 Dump 文件](#获取异常进程的-dump-文件)

# Process

## Process ID

### pidof

    pidof xxx

### grep

    ps -e|grep xxx
    第一列就为pid

#### ignore case

    grep -i javastreamer
    -> JavaStreamer

\-n 显示在文件中查找到的内容所在行
\-c 显示匹配次数

#### regular expression

正则表达式要用''括起来，与shell中通配符也有区别

^char 以某个字符开头  
char$ 以某个字符结尾  
. 任意一个字符  
\* 前面的字符零到多次  
.\* 匹配任意字符串（和shell中\*一样）  

    grep -i 'javastrea'
    -> JavaStrea[mer]

    grep -i 'javastrea.'
    -> JavaStream[er]

    grep -i 'javastrea.*'
    -> JavaStreamer

## Process numbers

ps -eLf|grep -i xxx|wc -l

## Process Status

### Process informations

    top -p pid1,pid2,pid3

### 占用cpu

* * *

查看/proc/\*/stat中的信息

* * *

    ps -aux|grep -i xxx|grep -v grep|awk '{print $3;}'  

该方法是错误的！！！！！！！！
`ps`命令所测出来的cpu是指   `一个进程占用cpu的时间/该进程运行的总时间`

* * *

而`top`命令表示`在一个固定的间隔时间内，某个进程使用的CPU时间占总CPU时间（即这段间隔时间）的比值。`  
默认显示为Irix模式，即为相对于单个cpu core的占用的时间，如双核机器需要将该数值除以2＊2计算，通过 shift+i可以进行切换；

    top -d 2|grep -i xxx >>top.txt

* * *

vmstat 的100%-idle == ps的cpu占用/cpu核心数 == top的cpu占用/cpu核心数/物理cpu数目

* * *

#### 定时

step1：写cron脚本文件，命名为crontest.cron。
15,30,45,59 \* \* \* \* echo "xgmtest....."     表示，每隔15分钟，执行一次打印命令 

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

---

#### **linux状态**  
1. 运行态、就绪态：在**就绪队列**中的进程，得到处理器就能运行
`RUNNING`：正在运行或者在就绪队列中等待运行的进程。**一个进程处于RUNNING状态，并不代表他一定在被执行。**由于在多任务系统中，各个就绪进程需要并发执行，所以在某个特定时刻，**这些处于RUNNING状态的进程之中，只有一个能得到`处理器`**，而其他进程必须在一个就绪队列中等待。  


2. 阻塞态：在**等待队列**中的进程，等待其他资源或者事件
`UNINTERRUPTABLE`:不可中断`阻塞状态`。处于这种状态的进程正在等待队列中，**当资源有效时，可由操作系统进行唤醒**，否则，将一直处于等待状态。  
`INTERRUPTABLE`：可中断`阻塞状态`。与不可中断阻塞状态一样，处于这种状态的进程在等待队列中，当资源有效时，可以有操作系统进行唤醒。与不可中断阻塞状态有所区别的是，**处于此状态中的进程亦可被其他进程的信号唤醒**。  

3. 停止态：挂起等待信号
`STOPPED`：挂起状态。进程被暂停，需要通过其它进程的信号才能被唤醒。导致这种状态的原因有两种。其一是受到相关信号(SIGSTOP,SIGSTP,SIGTTIN或SIGTTOU)的反应。其二是受到父进程ptrace调用的控制，而暂时将处理器交给控制进程。  

4. 僵死态
`ZOMBIE`：僵尸状态。表示进程结束但尚未消亡的一种状态。**此时进程已经结束运行并释放掉大部分资源，但尚未释放进程控制块，进程描述符仍然存在**。 

---

![img](./image_2.png)

---
#### linux进程状态标识

`R（Running）`(TASK_RUNNING)：可执行状态&运行状态；同一时刻可能有多个进程处于可执行状态，这些进程的task_struct结构（进程控制块）被放入**对应CPU的可执行队列**中（一个进程最多只能出现在一个CPU的可执行队列中）。进程调度器的任务就是从各个CPU的可执行队列中分别选择一个进程在该CPU上运行。

`S（Sleep）`(TASK_INTERRUPTIBLE)：该进程目前正在睡眠状态（idle），但可以被唤醒（signal）；处于这个状态的进程因为等待某某事件的发生（比如等待socket连接、等待信号量），而被挂起。这些进程的task_struct结构被放入**对应事件的等待队列**中。当这些事件发生时（由外部中断触发、或由其他进程触发），对应的等待队列中的一个或多个进程将被唤醒。

`D（uninterruptible）`(TASK_UNINTERRUPTIBLE)：不可被唤醒的状态，通常这个进程可能在等待I/O的情况（ex>打印）；**不可中断，指的并不是CPU不响应外部硬件的中断，而是指进程不响应异步信号。**

`T（traced or stopped）`(TASK_STOPPED or TASK_TRACED)：停止状态（stop）(TASK_STOPPED or TASK_TRACED)，可能是在工作控制（**后台暂停**，前台运行时按住Ctrl+z，程序退到后台并挂起，成为T状态。用jobs -l查看是stopped状态）或**跟踪**（traced）状态；  
当进程正在被跟踪时，它处于TASK_TRACED这个特殊的状态。“正在被跟踪”指的是进程暂停下来，等待跟踪它的进程对它进行操作。比如在gdb中对被跟踪的进程下一个断点，进程在断点处停下来的时候就处于TASK_TRACED状态。而在其他时候，被跟踪的进程还是处于前面提到的那些状态。而TASK_TRACED状态相当于在TASK_STOPPED之上多了一层保护，处于TASK_TRACED状态的进程不能响应SIGCONT信号而被唤醒。**只能等到调试进程通过ptrace系统(它提供了父进程可以观察和控制其子进程执行的能力)调用执行PTRACE_CONT、PTRACE_DETACH等操作（通过ptrace系统调用的参数指定操作），或调试进程退出，被调试的进程才能恢复TASK_RUNNING状态。**

`Z（Zombie）`(TASK_DEAD - EXIT_ZOMBIE)：“僵尸”状态，该进程已经终止但却无法被删除至内存外。不可被kill,　即不响应任务信号,　无法用SIGKILL杀死。通过ps命令查看其带有`defunct`的标志。**僵尸进程是一个早已死亡的进程，但在进程表 （processs table）中仍占了一个位置（slot）。**  
原因： 如果子进程先于父进程退出， 同时父进程又没有调用wait/waitpid，则该子进程将成为僵尸进程。

但是`如果该进程的父进程已经先结束了，那么该进程就不会变成僵尸进程`。因为每个进程结束的时候，系统都会扫描当前系统中所运行的所有进程，看看**有没有哪个进程是刚刚结束的这个进程的`子进程`**，如果是的话，就由Init进程来接管他，成为他的父进程，从而保证每个进程都会有一个父进程。而`Init进程会自动wait其子进程`，**因此被Init接管的所有进程都不会变成僵尸进程**。

---
#### T进程处理方法
可以通过`ps -e j|grep T`来查找T进程，然后手动杀死，这是一种强制关闭方法。不然应该按照
#### D和Z进程处理方法

-   cpu负载
    top中显示的load average 是对 CPU 负载的评估，其值越高，说明其任务队列越长（线程对CPU、磁盘、不可中断锁等系统资源的请求），处于等待执行的任务越多。   
    负载过高可能是D和Z进程多，而且不乏通过kill -9杀死。   
    一般会有1分钟、5分钟和15分钟三个指标，如果1分钟指标大于5分钟、15分钟指标，说明系统负载在增加，反之则在减少。如果这些值大于CPU核数，可能遇到了性能问题。

**D进程只能恢复其依赖的服务资源，让其得到资源才能完成释放**

 一般，为了防止产生僵尸进程，在fork子进程之后我们都要wait它们；同时，当子进程退出的时候，内核都会给父进程一个SIGCHLD信号，所以我们可以建立一个捕获SIGCHLD信号的信号处理函数，在函数体中调用wait（或waitpid），就可以清理退出的子进程以达到防止僵尸进程的目的。  

清除ZOMBIE（僵尸）进程可以使用如下方法：
1> kill –18 PPID （PPID是其父进程）
这个信号是告诉父进程，该子进程已经死亡了，请收回分配给他的资源。
2>如果不行则看能否终止其父进程（如果其父进程不需要的话）。先看其父进程又无其他子进程，如果有，可能需要先kill其他子进程，也就是兄弟进程。方法是：
kill –15 PID1 PID2(PID1,PID2是僵尸进程的父进程的其它子进程)。
然后再kill父进程：kill –15 PPID

---

## Process fd

ll /proc/pid/fd |wc -l

用lsof貌似不准

# 磁盘空间
linux磁盘空间分为3个部分：物理磁盘空间，inode节点占用磁盘空间，存放信号量的空间

查看可用inode结点
df -i


# Monitor tools

## linux

### htop

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

### gprof

[gprof](http://blog.csdn.net/stanjiang2010/article/details/5655143)只能找找函数调用关系和函数级别的性能，它是通过在既有程序中编译时插入代码来实现的,属于Instrumentation profiler。

-   编译时加上 -pg 选项  
-   程序运行完成后会在当前目录下生成gmon.out文件  
-   gprof ./tmp | gprof2dot.py |dot -Tpng -o report.png
-   或者 gprof ./tmp gmon.out >report.txt

问题：第三方库内的函数无法跟踪  

### valgrind

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

#### Callgrind

valgrind --tool=callgrind WebStreamer --conf-file=webstreamer.conf  
会生成一个callgrind.out.28982  
python gprof2dot.py -f callgrind -n10 -s callgrind.out.28982 > valgrind.dot  
dot -Tpng valgrind.dot -o valgrind.png  

gprof2dot.py -f callgrind callgrind.out.XXX |dot -Tpng -o report.png  

<http://blog.csdn.net/sunmenggmail/article/details/10543483>  

#### Memcheck

<https://www.cnblogs.com/findumars/p/5087306.html>  

valgrind --track-fds=yes --time-stamp=yes --leak-check=full  --log-file=res.txt WebStreamer --conf-file=webstreamer.conf

### perf

<https://www.ibm.com/developerworks/cn/linux/l-cn-perf1/index.html>

<https://mp.weixin.qq.com/s/npFqFeMSmuexoXJ2TACFsQ>

## windows

### ProcDump

ProcDump 是 System Internal 提供的一个专门用来监测程序 CPU 高使用率从而生成进程 Dump 文件的工具。ProcDump 可以根据系统的 CPU 使用率或者指定的性能计数器来针对特定进程生成一系列的 Dump 文件，以便调试者对事故原因进行分析。

#### 获取异常进程的 Dump 文件

1.  **通过【任务管理器】获取 Dump 文件，这样获取的是 MinDump。**
    选择进程，右键选择`创建转储文件`

2.  利用 WinDbg 的 adplus 获取 Dump 文件，这样获取的是 FullDump
3.  通过 DebugDiag 创建.NET 异常转储 Dump 文件
4.  **通过 ProcDump 抓取异常线程 Dump 文件**

```
procdump -c 70 -s 5 -ma -n 3 w3wp
  - 当系统 CPU 使用率持续 5 秒超过 70% 时，连续抓 3 个 Full Dump。
procdump outlook -p "\Processor(_Total)\% Processor Time" 80
  - 当系统 CPU 使用率超过 80%，抓取 Outlook 进程的 Mini Dump。
procdump -ma outlook -p "\Process(Outlook)\Handle Count" 10000
  - 当 Outlook 进程 Handle 数超过 10000 时抓取 Full Dump
procdump -ma 4572
  - 直接生成进程号位 4572 的 Full Dump。
```

然后[用WinDbg分析dump文件](http://www.infoq.com/cn/articles/architecture-practice-13-windbg)
