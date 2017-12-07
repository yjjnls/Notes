
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



### 监控某一进程的信息

```
top -p pid1,pid2,pid3
```