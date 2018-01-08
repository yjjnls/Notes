

## why
相比于protocal bufffer，flatbuffers不需要解析/解包的过程。同时，在工程中使用时，FlatBuffers的引用比Protocol Buffers方便很多，只需要包含两三个头文件即可。  

由于是以二进制形式缓存，**访问序列化数据时也无需数据解析过程**。即使对于层次化数据也不需要解析。多亏不需要初始化解析器（初始化意味着要建立复杂的字段映射）和解析数据，这些都是需要花费时间的。  
Flatbuffers数据**不需要分配比自身使用缓冲区还要多的内存**。我们不必像在JSON中那样为解析数据的整个层次分配额外对象。  




## struct和table
在生成C++代码时，struct的成员顺序会保持和IDL的定义顺序一致，如果有必要对齐，生成器会自动生成用于对齐的额外成员。
table的成员顺序是动态调整的，这和struct有区别。在生成C++代码时，生成器会自动调整为最佳顺序以保证它占用最小的内存空间。

structs are ideal for data structures that will not change, since they use less memory and have faster lookup


要想让数据结构具有可扩展性，需将数据结构定义为table，它是数据扩展的基础，FlatBuffers中的struct类型不支持扩展
如果想在后续的版本中删除数据结构中的某些字段，只要在将要删除的字段后面添加(deprecated)即可，当然需要保证删除的字段在之前版本的程序中不会引起程序崩溃
如果想在后续版本中向数据结构中添加某些字段，需添加到table中最后一个字段的后面，若是想table中随意位置添加字段，需如上面TextureData 的定义，给每个字段指明添加id:n（n从0开始）

```
table TextureData {
        image_size:int (id:0);
        image_data:[ubyte] (id:1);
        image_test:short(id:3);
        test_num2:int(id:2);
    }
```


schema编译器能够把给定的schema（在Java、C#、Python、GO和C++语言中的schema）生成为模型类，还可以把JSON转换成Flatbuffer的二进制文件。
