# start
redis-server&
# stop
redis-cli shutdown
 
# operations
## string
set [key] [val]
get [key]
del [key]

## list (a single list of string)
rpush [key] [val]
lrange [key] [pos1 pos2(-1 -> end of the list)]
lindex [key] [pos]
lpop [key]

## set (unordered set of string)
sadd [key] [val]
srem [key] [val]
smembers [key] //list all the members in the set
sismember [key] [val]

## hash (unordered set of key-val(<string,string>))
hset [key] [key1] [val1]
hget [key] [key1]
hgetall [key]
hdel [key] [key1]

## zset (ordered set of key-val(<number,string>))
zadd [key] [num(score)] [string(member)]
zrem [key] [string(member)]
zrange [key] [pos1 pos2 (-1 -> end of the set)] withscores
zrangebyscore [key] [score1 score2] withscores

并发？？？