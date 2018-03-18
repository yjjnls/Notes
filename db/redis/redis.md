# start
redis-server&
## redis client
redis-cli

# stop
redis-cli shutdown
 
# operations
这下面的key都可以看作某个类型的变量
## 字符串string（可以是字符串，整数和浮点数）
set [key] [val] （set x 5 / set x a）
get [key]
del [key]

## 列表list (a single list of string)
lpush/rpush [key] [val]  插入链表左端/右端
lrange [key] [pos1 pos2(-1 -> end of the list)]  获取链表在给定范围上的所有值
lindex [key] [pos]  获取链表某个位置上的元素
lpop/rpop [key]  弹出左端/右端的值
ltrim [key] [start] [end]  保留从start到end的元素

## 集合set (unordered set of string)元素不能重复
sadd [key] [val]
srem [key] [val]
smembers [key] //列出集合中的所有元素
sismember [key] [val]

## 散列hash (unordered set of key-val(<string,string>))
hset [key] [key1] [val1]
hget [key] [key1]
hgetall [key]
hdel [key] [key1]

## 有序集合zset (ordered set of key-val(<number,string>))
按照score值排列

zadd [key] [num(score)] [string(member)]
zrem [key] [string(member)]
zrange [key] [pos1 pos2 (-1 -> end of the set)] withscores    列出所有的成员以及他们的score，没有withoutscores就只列出成员而不列出分数。
pos1为起始元素位置，pos2为end元素位置，都是以0为基础，i表示第i+1个元素，-i表示倒数第i个元素，比如 0 -2 表示从第1个元素到倒数第二个元素

zrangebyscore [key] [score1 score2] withscores
zscore [key] member  列出某个member的score值

并发？？？

查看有多少个连接    client info
查看所有key        keys *
清除所有数据       flushdb（当前db） flushall（多有db）默认有16个db

zadd时间复杂度怎么算的
zrank 如何在logN时间复杂度内获取某个元素在第几个位置

## 事务
通过multi和exec命令来实现  
multi之后的命令都会返回queued，存入了队列，直到执行exec才会正式执行。
**可以对要进行的原子操作的数据进行watch操作，watch在multi之前，如果执行到multi时，数据已经发生了变化，则中断事务。**比如某个函数要对某个id的商品进行操纵，从进入函数开始，到执行exec，该id的商品可能已经被操作修改了，甚至删除了，所以这里应该先watch，如果发生了变化，那么就中断后续事务。**在数据被其他客户端抢先修改的情况下，通知客户端，这叫乐观锁。而关系数据库中，先加锁再操作，叫悲观锁，阻塞等待时间长。**  
如果要取消事务，那么就用discard。可以在watch之后，multi之前，也可以在multi之后，exec之前。

## ★锁
redis为单线程，所有的操作命令都是原子的，但是多个操作就不是原子的，需要靠另外的机制来保障。    
事务可以保证一组命令执行不被打断，就像原子的一样，但如果操作涉及多个数据存储部分，或者后一操作依赖前一操作的数据，那么就要用watch。    
watch本质上是保证从进入程序，到exec执行这段时间，数据没有更改。所以也可以用分布式锁来实现。   

锁还可以实现更加细粒度的操作，可以有效避免watch带来的竞争过多而导致的延迟剧增甚至无法执行的问题。

## 持久化
### 快照持久化
SAVE 和 BGSAVE 两个命令都会调用 rdbSave 函数，但是SAVE是阻塞调用，BGSAVE是fork一个子进程异步完成。  

* 发送save和bgsave来创建快照
* 配置了save选项，满足时会出发bgsave选项
* shutdown或者term关闭，会执行save，也可以设置nosave
* 两台redis服务器连接，发送sync复制时，会执行bgsave
* **快照持久化适合允许丢失一部分数据的应用。**

在数据量大时，可以考虑关闭自动保存，在redis.conf中将save去掉，数据库保存的名字通过dbfilename设置，默认为dump.rdb。保存目录通过dir设置，默认为当前启动redis-server的目录。

* RDB文件的载入一般情况是自动的，redis服务器启动的时候，redis服务器再启动的时候如果检测到RDB文件的存在，那么redis会自动载入这个文件。
* 如果服务器开启了AOF持久化，那么服务器会优先使用AOF文件来还原数据库状态。
* RDB是通过保存键值对来记录数据库状态的，采用copy on write的模式，每次都是全量的备份。

### AOF持久化
执行写命令时，将被执行的命令复制到硬盘里面。  
AOF文件会不断地增大，可以用BGREWRITEAOF来fork一个子进程来重写AOF文件，


## 复制
主机可以向从机发送更新，让从机来处理读请求。当读请求的重要性明显高于写请求的重要性时，当读请求的数量远远超出一台redis服务器可以处理范围时。
[主从复制与集群配置](http://blog.csdn.net/u011204847/article/details/51307044)

## 批量操作
* mget、mset等一次传递多个参数
* 使用pipeline(false)，不用事务的原子性，但是用一条request请求将所有命令一起送过去，由redis内部fake出一个client做批量执行操作

## 性能测试
redis-benchmark模拟客户端来进行模拟每个命令能跑多快，但是benchmark不处理返回结果，所以当个客户端达到redia-benchmark的50%~60%就算正常。