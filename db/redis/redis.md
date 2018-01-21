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
lrange [key] [pos1 pos2(-1 -> end of the list)] 获取链表在给定范围上的所有值
lindex [key] [pos] 获取链表某个位置上的元素
lpop/rpop [key]  弹出左端/右端的值，

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
查看所有key        keys ×
清除所有数据       flushdb（当前db） flushall（多有db）默认有16个db