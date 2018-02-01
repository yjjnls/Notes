# consistent hashing
## 基本场景
比如你有 N 个 cache 服务器（后面简称 cache ），那么如何将一个对象 object 映射到 N 个 cache 上呢，你很可能会采用类似下面的通用方法计算 object 的 hash 值，然后均匀的映射到到 N 个 cache；

hash(object)%N

一切都运行正常，再考虑如下的两种情况；  
1 一个 cache 服务器 m down 掉了（在实际应用中必须要考虑这种情况），这样所有映射到 cache m 的对象都会失效，怎么办，需要把 cache m 从 cache 中移除，这时候 cache 是 N-1 台，映射公式变成了 hash(object)%(N-1) ；  
2 由于访问加重，需要添加 cache ，这时候 cache 是 N+1 台，映射公式变成了 hash(object)%(N+1) ；  

1 和 2 意味着什么？**这意味着突然之间几乎所有的 cache 都失效了。对于服务器而言，这是一场灾难，洪水般的访问都会直接冲向后台服务器**；


## 单调性
Hash 算法的一个衡量指标是**单调性**（ Monotonicity ），定义如下：  
    单调性是指如果已经有一些内容通过哈希分派到了相应的缓冲中，`又有新的缓冲加入到系统中`。**哈希的结果应能够保证原有已分配的内容可以被映射到新的缓冲中去，而不会被映射到旧的缓冲集合中的其他缓冲区。** 
容易看到，上面的简单 hash 算法 hash(object)%N 难以满足单调性要求。  


consistent hashing 是一种 hash 算法，简单的说，**在移除 / 添加一个 cache 时，它能够尽可能小的改变已存在 key 映射关系，尽可能的满足单调性的要求。**

## 平衡性
考量 Hash 算法的另一个指标是**平衡性** (Balance) ，定义如下： 
　　平衡性是指哈希的结果能够尽可能分布到所有的缓冲中去，这样可以使得所有的缓冲空间都得到利用。

虚拟结点是实际结点的一些复制，比如结点A复制为A1A2，然后和B1B2交错分布，这样数据在不同节点的分布就比较均匀。

----
一致性哈希只是解决少量增减服务器导致的大量震荡问题，要保持会话的延续不是他可以解决的。


<!-- http://shuofenglxy.iteye.com/blog/1188422 -->
<!-- http://blog.csdn.net/dellme99/article/details/14168263 -->
<!-- http://blog.csdn.net/dellme99/article/details/14162159 -->
<!-- http://blog.csdn.net/dellme99/article/details/15432249 -->
http://blog.csdn.net/dellme99/article/details/51884292
<!-- http://blog.csdn.net/dellme99/article/details/51884307 -->
<!-- http://blog.csdn.net/dellme99/article/category/1690035 -->
<!-- http://duanple.blog.163.com/blog/static/70971767201311810939564/ -->
http://blog.csdn.net/kevinlynx/article/details/40122407
<!-- http://blog.csdn.net/aesop_wubo/article/details/20401431 -->
<!-- http://blog.csdn.net/cloudresearch/article/details/23127985 -->
<!-- http://blog.jobbole.com/95632/ -->
https://mp.weixin.qq.com/s/kzmTKKH-t6tpJ97fa6TYPg
https://mp.weixin.qq.com/s/i1pnrORZzec6Zp0tmljD8Q
<!-- https://zhuanlan.zhihu.com/p/26762775 -->
<!-- https://zhuanlan.zhihu.com/p/25423971 -->

