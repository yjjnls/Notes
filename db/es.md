*   索引是ElasticSearch存放数据的地方，可以理解为关系型数据库中的一个数据库。事实上，我们的数据被存储和索引在分片(shards)中，索引只是一个把一个或多个分片分组在一起的逻辑空间。然而，这只是一些内部细节——我们的程序完全不用关心分片。对于我们的程序而言，文档存储在索引(index)中。剩下的细节由Elasticsearch关心既可。（索引的名字必须是全部小写，不能以下划线开头，不能包含逗号）
*   类型用于区分同一个索引下不同的**数据类型**,相当于关系型数据库中的表。在Elasticsearch中，我们使用相同类型(type)的文档表示相同的“事物”，因为他们的数据结构也是相同的。每个类型(type)都有自己的映射(mapping)或者结构定义，就像传统数据库表中的列一样。所有类型下的文档被存储在同一个索引下，但是类型的映射(mapping)会告诉Elasticsearch不同的文档如何被索引。
*   文档是ElasticSearch中存储的实体，类比关系型数据库，**每个文档相当于数据库表中的一行数据**。 在Elasticsearch中，文档(document)这个术语有着特殊含义。它特指最顶层结构或者根对象(root object)序列化成的JSON数据（以唯一ID标识并存储于Elasticsearch中）。
*   文档由字段组成，相当于关系数据库中列的属性，不同的是ES的不同文档可以具有不同的字段集合。 


```c
Relational DB -> Databases -> Tables -> Rows -> Columns
Elasticsearch -> Indices -> Types -> Documents -> Fields
```

一个文档不只有数据。它还包含了元数据(metadata)——关于文档的信息。三个必须的元数据节点是：

*   _index：索引
*   _type：类型
*   _id：id仅仅是一个字符串，它与_index和_type组合时，就可以在Elasticsearch中**唯一标识一个文档**。当创建一个文档，你可以自定义_id，也可以让Elasticsearch帮你自动生成。

*   类似的数据放在一个索引，非类似的数据放不同索引：product index（包含了所有的商品），sales index（包含了所有的商品销售数据），inventory index（包含了所有库存相关的数据）。如果你把比如product，sales，human resource（employee），全都放在一个大的index里面，比如说company index，不合适的。
*   index中包含了很多类似的document：类似是什么意思，其实指的就是说，这些document的fields很大一部分是相同的，你说你放了3个document，每个document的fields都完全不一样，这就不是类似了，就不太适合放到一个index里面去了。
*   索引名称必须是小写的，不能用下划线开头，不能包含逗号：product，website，blog
 

# 查看集群健康状态
curl -X GET "172.16.65.198:9200/_cat/health?v"

# 查看索引
curl -X GET "172.16.65.198:9200/_cat/indices?v"

# 查询索引库index1中的类型type1
curl -X GET "172.16.65.198:9200/index1/type1/_search?pretty"

# 创建索引
## PUT创建

索引：blog，类型article，文档：doc1，-d后面的消息体中表示具体的field

```sh
curl -X PUT http://172.16.65.198:9200/blog/article/doc1 -d '
{
      "id": "1",
      "title": "New version of Elasticsearch released!",
      "content": "Version 1.0 released today!",
      "priority": 10,
      "tags": ["announce", "elasticsearch", "release"]
}'

{"_index":"blog","_type":"article","_id":"doc1","_version":1,"result":"created","_shards":{"total":2,"successful":1,"failed":0},"created":true}

```

PUT是更新类型，每次都要指定文档_id，每操作一次，_version+1。

如果PUT操作中加上_create，那么就是指定为创建操作，如果两次_id一样，那么会报错。

## POST创建

用POST来创建数据可以指定_id，也可以不指定，此时如果存在相同数据则是修改，不指定id的话会随机生成id，且每次执行都会生成新数据。

## Post与Put的区别

Put是等幂操作，即无论执行多少次结果都一样，例如DEL无论删除多少次索引库中的结果都一样，put只要指定了id且数据不变无论执行多少次索引库中的数据都不变，只有version会变化。

Post每次执行都会产生新数据。