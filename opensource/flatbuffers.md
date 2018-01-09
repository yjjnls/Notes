# flatbuffers
序列化主要用在不同语言间的通信，将某个语言的数据结构序列化成一个公共定义的结构，然后另一种语言解析之后成为该语言可以处理的结构。

## why
相比于protocal bufffer，flatbuffers 是 zero-copy + Random-access reads 版本的 protobuf，不需要解析/解包的过程。同时，在工程中使用时，FlatBuffers的引用比Protocol Buffers方便很多，只需要包含两三个头文件即可。  

 
* 对序列化数据的访问**不需要打包和拆包**——它将序列化数据存储在缓存中，这些数据既可以存储在文件中，又可以通过网络原样传输，而没有任何解析开销；(这是最主要的原因，ProtoBuffer、JSON等均需要拆包和解包)
* 内存效率和速度——访问数据时的**唯一内存需求就是缓冲区**，**不需要额外的内存分配**。 这里可查看详细的基准测试；
* 扩展性、灵活性——它支持的可选字段意味着不仅能获得很好的**前向/后向兼容性**（对于长生命周期的游戏来说尤其重要，因为不需要每个新版本都更新所有数据）；
* 最小代码依赖——仅仅需要自动生成的少量代码和一个**单一的头文件依赖**，很容易集成到现有系统中。再次，看基准部分细节；
* 强类型设计——尽可能使错误出现在编译期，而不是等到运行期才手动检查和修正；
* 使用简单——生成的C++代码提供了简单的访问和构造接口；而且如果需要，通过一个可选功能可以用来在运行时高效解析Schema和类JSON格式的文本；
* 跨平台——支持C++11、Java，而不需要任何依赖库；在最新的gcc、clang、vs2010等编译器上工作良好；


## schema
### struct和table
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

### build
cmake -G "Unix Makefiles"  
make  
在根目录下会生成 flatc程序，用于schema的编译  

cmake -DCMAKE_INSTALL_PREFIX:PATH=/home/coder4/soft/flatbuffers -G "Unix Makefiles"  
make && make install

### tests
cd smaple  
./../flatc --cpp monster.fbs  
会生产monster_generated.h

```python
Usage: ./flatc [OPTION]... FILE... [-- FILE...]
  --binary     -b    Generate wire format binaries for any data definitions.
  --json       -t    Generate text output for any data definitions.
  --cpp        -c    Generate C++ headers for tables/structs.
  --go         -g    Generate Go files for tables/structs.
  --java       -j    Generate Java classes for tables/structs.
  --js         -s    Generate JavaScript code for tables/structs.
  --ts         -T    Generate TypeScript code for tables/structs.
  --csharp     -n    Generate C# classes for tables/structs.
  --python     -p    Generate Python files for tables/structs.
  --php              Generate PHP files for tables/structs.
  --jsonschema       Generate Json schema.
  -o PATH            Prefix PATH to all generated files.
  -I PATH            Search for includes in the specified path.
  -M                 Print make rules for generated files.
  --version          Print the version number of flatc and exit.
  --strict-json      Strict JSON: field names must be / will be quoted,
                     no trailing commas in tables/vectors.
  --allow-non-utf8   Pass non-UTF-8 input through parser and emit nonstandard
                     \x escapes in JSON. (Default is to raise parse error on
                     non-UTF-8 input.)
  --defaults-json    Output fields whose value is the default when
                     writing JSON
  --unknown-json     Allow fields in JSON that are not defined in the
                     schema. These fields will be discared when generating
                     binaries.
  --no-prefix        Don't prefix enum values with the enum type in C++.
  --scoped-enums     Use C++11 style scoped and strongly typed enums.
                     also implies --no-prefix.
  --gen-includes     (deprecated), this is the default behavior.
                     If the original behavior is required (no include
                     statements) use --no-includes.
  --no-includes      Don't generate include statements for included
                     schemas the generated file depends on (C++).
  --gen-mutable      Generate accessors that can mutate buffers in-place.
  --gen-onefile      Generate single output file for C# and Go.
  --gen-name-strings Generate type name functions for C++.
  --gen-object-api   Generate an additional object-based API.
  --cpp-ptr-type T   Set object API pointer type (default std::unique_ptr)
  --cpp-str-type T   Set object API string type (default std::string)
                     T::c_str() and T::length() must be supported
  --gen-nullable     Add Clang _Nullable for C++ pointer. or @Nullable for Java
  --object-prefix    Customise class prefix for C++ object-based API.
  --object-suffix    Customise class suffix for C++ object-based API.
                     Default value is "T"
  --no-js-exports    Removes Node.js style export lines in JS.
  --goog-js-export   Uses goog.exports* for closure compiler exporting in JS.
  --go-namespace     Generate the overrided namespace in Golang.
  --go-import        Generate the overrided import for flatbuffers in Golang.
                     (default is "github.com/google/flatbuffers/go")
  --raw-binary       Allow binaries without file_indentifier to be read.
                     This may crash flatc given a mismatched schema.
  --proto            Input is a .proto, translate to .fbs.
  --grpc             Generate GRPC interfaces for the specified languages
  --schema           Serialize schemas instead of JSON (use with -b)
  --bfbs-comments    Add doc comments to the binary schema files.
  --conform FILE     Specify a schema the following schemas should be
                     an evolution of. Gives errors if not.
  --conform-includes Include path for the schema given with --conform
    PATH
  --include-prefix   Prefix this path to any generated include statements.
    PATH
  --keep-prefix      Keep original prefix of schema include statement.
  --no-fb-import     Don't include flatbuffers import statement for TypeScript.
  --no-ts-reexport   Don't re-export imported dependencies for TypeScript.
  --reflect-types    Add minimal type reflection to code generation.
  --reflect-names    Add minimal type/name reflection.
FILEs may be schemas (must end in .fbs), or JSON files (conforming to preceding
schema). FILEs after the -- must be binary flatbuffer format files.
Output files are named using the base file name of the input,
and written to the current directory or the path given by -o.
example: ./flatc -c -b schema1.fbs schema2.fbs data.json
```