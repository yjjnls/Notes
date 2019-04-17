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

PUT是更新类型，每次都要指定文档`_id`，每操作一次，_version+1。

如果PUT操作中加上_create，那么就是指定为创建操作，如果两次`_id`一样，那么会报错。

注意这里文档的id为`_id`，而`"id": "1"`是文档中的一个属性。

## POST创建

用POST来创建数据可以指定_id，也可以不指定，此时如果存在相同数据则是修改，不指定id的话会随机生成id，且每次执行都会生成新数据。

## Post与Put的区别

Put是等幂操作，即无论执行多少次结果都一样，例如DEL无论删除多少次索引库中的结果都一样，put只要指定了id且数据不变无论执行多少次索引库中的数据都不变，只有version会变化。

Post每次执行都会产生新数据。

# 查询
## 查询类型中的所有文档
查询索引库blog中的类型article，结果会显示article中所有的文档（即查询数据库blog中的表article，显示其所有行数据，行数据是类似的，但并不是像sql中完全一致的）。

```sh
curl -X GET "172.16.65.198:9200/blog/article/_search?pretty"
```

这里用到了`_search`关键词，同理，查询所有索引库：

```sh
curl -X GET "172.16.65.198:9200/_search?pretty"
```

查询索引中所有类型

```sh
curl -X GET "172.16.65.198:9200/blog/_search?pretty"
```

## 查询指定文档
查询blog中article类型中，_id为doc1的文档

```sh
curl -X GET "172.16.65.198:9200/blog/article/doc1?pretty"
```

## 根据条件查询
查询blog索引、article类型中查询id为2的数据，这里是指文档中的具体数据

```sh
curl -X GET "172.16.65.198:9200/blog/article/_search?q=id:2&pretty"
```

查询文档编号为doc1的数据

```sh
curl -X GET "172.16.65.198:9200/blog/article/_search?q=_id:doc1&pretty"
```

**查询id为2的数据时，es用倒排索引来查询，而sql把id作为B+树的索引来查询。**

# 更新数据
## 局域更新

## 全局更新

```sh
curl -X POST http://172.16.65.198:9200/blog/article/doc1 -d '
{
      "id": "2"
}'

curl -X PUT http://172.16.65.198:9200/blog/article/doc1 -d '
{
      "id": "2"
}'

```
覆盖原来的文档，直接生成一个新的doc

全部更新，是直接把之前的老数据，标记为删除状态，然后，再添加一条更新的。

局域更新，只是修改某个字段。

# 删除

删除文档并不会立即生效，只会将其标记为已删除，当后续添加更多索引时才会在后台删除。

## 根据_id删除
删除_id为1的doc

```sh
curl -X DELETE "http://172.16.65.198:9200/blog/article/1"
```

待确认：
```sh
curl -X DELETE "http://172.16.65.198:9200/blog/article/_query?q=id:2&pretty"

```


```sh
POST twitter/_delete_by_query
{
  "query": { 
    "match": {
      "message": "some message"
    }
  }
}

```

```sh
curl -X POST http://172.16.65.198:9200/blog/article/_delete_by_query -d '
{
    "query":{
        "bool":{
            "should":[
               {"match":{"prioritys": 10}} 
            ]
        }
    }
}'
# _delete_by_query只会有一条回复
{"took":62,"timed_out":false,"total":1,"deleted":1,"batches":1,"version_conflicts":0,"noops":0,"retries":{"bulk":0,"search":0},"throttled_millis":0,"requests_per_second":-1.0,"throttled_until_millis":0,"failures":[]}
# 再次运行
{"took":1,"timed_out":false,"total":0,"deleted":0,"batches":0,"version_conflicts":0,"noops":0,"retries":{"bulk":0,"search":0},"throttled_millis":0,"requests_per_second":-1.0,"throttled_until_millis":0,"failures":[]}
```

query匹配语句：https://blog.csdn.net/HaixWang/article/details/80207769

# 批量操作
## bulk的格式
*   action：index/create/update/delete
*   metadata：_index, _type, _id
*   request body：_source (删除操作不需要加request body)
                   { action: { metadata }}
                   { request body        }

## create 和index的区别
　　如果数据存在，使用create操作失败，会提示文档已经存在，使用index则可以成功执行。

## bulk一次最大处理多少数据量？
　　bulk会把将要处理的数据载入内存中，所以数据量是有限制的，最佳的数据量不是一个确定的数值，它取决于你的硬件，你的文档大小以及复杂性，你的索引以及搜索的负载。    
　　一般建议是1000-5000个文档，如果你的文档很大，可以适当减少队列，大小建议是5-15MB，默认不能超过100M，可以在es的配置文件（即$ES_HOME下的config下的elasticsearch.yml）中。


```sh
curl -X POST "http://172.16.65.198:9200/blog/_bulk" -H 'Content-Type: application/json' -d'
{ "index" : { "_type" : "test", "_id" : "1" } }
{ "field1" : "value1" }
'

{"took":75,"errors":false,"items":[{"index":{"_index":"blog","_type":"test","_id":"1","_version":1,"result":"created","_shards":{"total":2,"successful":1,"failed":0},"created":true,"status":201}}]}
# 第二次执行
{"took":58,"errors":false,"items":[{"index":{"_index":"blog","_type":"test","_id":"1","_version":2,"result":"updated","_shards":{"total":2,"successful":1,"failed":0},"created":false,"status":200}}]}
```

```sh
curl -X POST "http://172.16.65.198:9200/blog/_bulk" -H 'Content-Type: application/json' -d'
{ "create" : { "_type" : "test", "_id" : "1" } }
{ "field1" : "value1" }

{"took":0,"errors":true,"items":[{"create":{"_index":"blog","_type":"test","_id":"1","status":409,"error":{"type":"version_conflict_engine_exception","reason":"[test][1]: version conflict, document already exists (current version [2])","index_uuid":"dpaC9p1IR4iP5k7mMKy14g","shard":"3","index":"blog"}}}]}
```

**document already exists (current version [2])**

```sh
curl -X POST http://172.16.65.198:9200/blog/_bulk?pretty  -d '
{ "index":{ "_type": "article", "_id": "doc6" } }
{ "id": "6", "title": "New version of Elasticsearch released!", "content": "Version 1.0 released today!", "priority": 10, "tags": ["announce", "elasticsearch", "release"] }
{ "create":{ "_type": "article", "_id": "doc6" } }
{ "id": "6", "title": "New version of Elasticsearch released!", "content": "Version 1.0 released today!", "priority": 10, "tags": ["announce", "elasticsearch", "release"] }
{ "index":{ "_type": "article", "_id": "doc6" } }
{ "id": "6", "title": "New version of Elasticsearch released!", "content": "Version 1.0 released today!", "priority": 10, "tags": ["announce", "elasticsearch", "release"] }
'

{
  "took" : 63,
  "errors" : true,
  "items" : [
    {
      "index" : {
        "_index" : "blog",
        "_type" : "article",
        "_id" : "doc6",
        "_version" : 16,
        "result" : "updated",
        "_shards" : {
          "total" : 2,
          "successful" : 1,
          "failed" : 0
        },
        "created" : false,
        "status" : 200
      }
    },
    {
      "create" : {
        "_index" : "blog",
        "_type" : "article",
        "_id" : "doc6",
        "status" : 409,
        "error" : {
          "type" : "version_conflict_engine_exception",
          "reason" : "[article][doc6]: version conflict, document already exists (current version [16])",
          "index_uuid" : "dpaC9p1IR4iP5k7mMKy14g",
          "shard" : "4",
          "index" : "blog"
        }
      }
    },
    {
      "index" : {
        "_index" : "blog",
        "_type" : "article",
        "_id" : "doc6",
        "_version" : 17,
        "result" : "updated",
        "_shards" : {
          "total" : 2,
          "successful" : 1,
          "failed" : 0
        },
        "created" : false,
        "status" : 200
      }
    }
  ]
}
```