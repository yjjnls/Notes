## 集合操作set
### 交集
SINTER
O(N * M)， N 为给定集合当中基数最小的集合， M 为给定集合的个数。

遍历个数最小的集合N，到其他M-1个中查找

### 差集
SDIFF 存在于第一个集合，但不存在于其他集合
O(N)， N 是所有给定集合的成员数量之和。

？？？为什么时间复杂度和SINTER不一样

### 并集
SUNION 所有集合的并集
O(N)， N 是所有给定集合的成员数量之和。

## 有序集合zset
### 交集
ZINTERSTORE
ZINTERSTORE destination numkeys key [key ...] [WEIGHTS weight [weight ...]] [AGGREGATE SUM|MIN|MAX]
默认采用sum，即求交集后对其进行相加，并以相加后的结果作为排序分值。