# Solidity
Solidity 是一门面向合约的、为实现智能合约而创建的高级编程语言。这门语言受到了 C++，Python 和 Javascript 语言的影响，设计的目的是能在以太坊虚拟机（EVM）上运行。

Solidity 是静态类型语言，支持继承、库和复杂的用户定义类型等特性。

[官方文档](https://solidity.readthedocs.io/en/develop/index.html) [中文版](https://solidity-cn.readthedocs.io/zh/develop/)

目前尝试 Solidity 编程的最好的方式是使用 [Remix](https://remix.ethereum.org/) （需要时间加载，请耐心等待）。Remix 是一个基于 Web 浏览器的 IDE，它可以让你编写 Solidity 智能合约，然后部署并运行该智能合约。

Solidity中合约的含义就是一组代码( 它的函数 )和数据( 它的状态 )，它们位于以太坊区块链的一个特定地址上。


### delete
删除用于释放空间，释放空间后会返回一定gas。**删除本质是对一个变量赋初值。**

*   基本类型: 对于基本类型，使用delete会设置为对应的初始值
*   枚举类型: 删除枚举类型时，会将其值重置为序号为0的值
*   函数: 不能删除
*   struct: 会将其中的所有成员变量一一置为初值
*   mapping:  不能直接删除，但可以删除某一个(key, value)
*   mapping in struct: 会跳过该mapping
*   定长数组: 将元素全部置为初值 （bytes32）
*   变长数组: 清除元素，长度置0 （uint[]）
*   删除某个元素: 将该元素置为初值，但并没有删除该元素
*   storage: 报错

当数组或者mapping非常大的时候，删除或者维护他们非常耗gas。清理空间，可以获得gas的返还。但无特别意义的数组的整理和删除，只会消耗更多gas，需要在业务实现上进行权衡。

所以一般大对象不主动清理，能复用就复用，忽略已经用过的位置。如果大对象是在某个事件发生时，一次性使用，然后需要回收的。一个更有效的方式是，在发生某个事件时，创建一个新合约，在新合约完成逻辑，完成后，让合约suicide。清理合约占用空间返还的gas就退还给了调用者，来节省主动遍历删除消耗的额外gas。