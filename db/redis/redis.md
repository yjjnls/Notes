# start
redis-server&
## redis client
redis-cli

# stop
redis-cli shutdown
 
# operations
这下面的key都可以看作某个类型的变量
## 字符串string（可以是字符串，整数和浮点数）
set [key] [val]
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
zadd [key] [num(score)] [string(member)]
zrem [key] [string(member)]
zrange [key] [pos1 pos2 (-1 -> end of the set)] withscores
zrangebyscore [key] [score1 score2] withscores

并发？？？