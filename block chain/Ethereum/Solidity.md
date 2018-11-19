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


### 异常处理
Solidity是通过回退状态的方式来处理错误。**发生异常时会撤消当前调用（及其所有子调用）所改变的状态，同时给调用者返回一个错误标识**。注意捕捉异常是不可能的，因此没有try … catch…。

* 传统方式 if throw
```
contract HasAnOwner {
    address owner;

    function useSuperPowers(){ 
        if (msg.sender != owner) { throw; }
        // do something only the owner should be allowed to do
    }
}
```
throw将抛出“返回无效操作代码错误”，**回滚所有状态改变**，而且消耗掉剩下的gas。

* 新的方式
`if(msg.sender != owner) { throw; }`

完全等价于如下三种形式：

* `if(msg.sender != owner) { revert(); }`
* `assert(msg.sender == owner);`
* `require(msg.sender == owner);`

#### assert 和 require
assert函数只能用于测试内部错误，并检查不变量。
应该使用require函数来确认input或合约状态变量满足条件，或者验证调用外部合约的返回值。

assert()返回0xfe来表示异常，而require()返回0xfd表示异常。

此外assert触发后会消耗所有gas，而require触发后会返回剩余gas。**他们触发后都会停止当前操作，回退状态，交易会失败，event不会记录到log中**。

当发生require类型的异常时，Solidity会执行一个回退操作（指令0xfd）。
当发生assert类型的异常时，Solidity会执行一个无效操作（指令0xfe）。

[实际测试例子](https://stackoverflow.com/a/48847724/10605675)

[实际例子2](https://learnblockchain.cn/2018/04/07/solidity-errorhandler/)

assert(false) compiles to 0xfe, which is an invalid opcode, using up all remaining gas, and reverting all changes.

require(false) compiles to 0xfd which is the REVERT opcode, meaning it will refund the remaining gas. The opcode can also return a value.

* Use  `assert()`  to:
    * check for overflow/underflow
    * check invariants
    * validate contract state  *after*  making changes
    * avoid conditions which should never, ever be possible.
    * Generally, you should use  `assert`  less often
    * Generally, it will be use towards the end of your function.

* Use  `require()`  to:
    * Validate user inputs
    * Validate the response from an external contract
    ie. use  `require(external.send(amount))`
    * Validate state conditions prior to executing state changing operations, for example in an  `owned` contract situation
    * Generally, you should use  `require`  more often,
    * Generally, it will be used towards the beginning of a function.

另外，如果我们正确使用assert，有一个**Solidity分析工具就可以帮我们分析出智能合约中的错误**，帮助我们发现合约中有逻辑错误的bug。

#### revert
revert的用法和throw很像，也会撤回所有的状态转变。但是它有两点不同：

1. 它允许你返回一个值；

2. 它会把所有剩下的gas退回给caller

调用起来就像这样子：

```
revert(‘Something bad happened’);

require(condition, ‘Something bad happened’);
```

ref  
http://blockgeek.org/t/topic/554     
http://blockgeek.org/t/assert-require/1523/3     
https://ethereum.stackexchange.com/questions/15166/difference-between-require-and-assert-and-the-difference-between-revert-and-thro     


### ABI
在 Ethereum 智能合约可以被大家使用前，必须先被部署到区块链上。从智能合约的原始码到使用智能合约，大概包含几个步骤：

*   **撰写** 智能合约的原始码（一般是用 Solidity 写）
*   **编译** 智能合约的原始码成可在 EVM 上执行的 bytecode（binary code）。同时可以透过编译取得智能合约的 ABI
*   **部署** 智能合约，实际上是把 bytecode 储存在链上（透过一个 transaction），并取得一个专属这个 合约的地址
*   如果要写个程式 **呼叫** 这个智能合约，就要把资料发送到这个合约的地址（一样透过一个 transaction）。Ethereum 节点会根据输入的资料，选择要执行合约中的哪一个 function 和要输入的参数

而要如何知道这个智能合约提供哪些 function 以及应该要传入什么样的参数呢？这些资讯就是记录在智能合约的 ABI。

Ethereum 智能合约 ABI 用一个 array 表示，其中会包含数个用 JSON 格式表示的 Function 或 Event 。根据最新的 Solidity 文件：

#### Function

共有 7 个栏位：

* `name`  ：a string，function 名称
* `type`  ：a string，“function”, “constructor”, or “fallback”
* `inputs`  ：an array，function 输入的参数，包含：
  * `name`  ：a string，参数名称
  * `type`  ：a string，参数的 data type(e.g. uint256)
  * `components`  ：an array，如果输入的参数是 tuple(struct) type 才会有这个栏位。描述 struct 中包含的资料型态
* `outputs`  ：an array，function 的回传值，和  `inputs`  使用相同表示方式。如果没有回传值可忽略，值为  `[]`
* `payable`  ：  `true`  ，如果 function 可收 Ether，预设为  `false`
* `constant`  ：  `true`  ，如果 function 不会改写区块链的状态，反之为  `false`
* `stateMutability`  ：a string，其值可能为以下其中之一：“pure”（不会读和写区块链状态）、“view”（会读区块链状态，但不会改写区块链状态）、“payable” and “nonpayable”（会改写区块链状态，且如可收 Ether 为 “payable”，反之为 “nonpayable”）

> 仔细看会发现  `payable`  和  `constant`  这两个栏位所描述的内容，似乎已包含在  `stateMutability`  中。

事实的确是这样，在 [Solidity v0.4.16](https://link.jianshu.com/?t=https%3A%2F%2Fgithub.com%2Fethereum%2Fsolidity%2Freleases) 中把  `constant`  这个修饰 function 的 key words 分成  `view`  （neither reads from nor writes to the state）和  `pure` （does not modify the state），并从 v0.4.17 开始 Type Checker 会强制检查。  `constant`  改为只用来修饰不能被修改的 variable。并在 ABI 中加入 `stateMutability`  这个栏位统一表示，  `payable`  和  `constant`  目前保留是为了  **向后兼容**  。

#### Event

共有 4 个栏位：

* `name`  : a string，event 的名称
* `type`  : a string，always “event”
* `inputs`  : an array，输入的参数，包含：
  * `name`  : a string，参数名称
  * `type`  : a string，参数的 data type(e.g. uint256)
  * `components`  : an array，如果输入的参数是 tuple(struct) type 才会有这个栏位。描述 struct 中包含的资料型态
  * `indexed`  :  `true`  ，如果这个参数被宣告为 indexed（被存在 log 的 topics 中），反之为  `false`  （储存在 log 的 data 中）
* `anonymous`  :  `true`  ，如果 event 被宣告为 anonymous

**更新智能合约状态需要发送 transaction，transaction 需要等待验证，所以更新合约状态是非同步的，无法马上取得回传值。使用 Event 可以在状态更新成功后，将相关资讯记录到 Log，并让监听这个 Event 的 DApp 或任何使用介面收到通知。每一笔 transaction 都有对应的 Log。**

所以简单来说，Event 可用来：
1. 取得 function 更新合约状态后的回传值 
2. 也可作为合约另外的储存空间。

Event 的参数分为：有  `indexed`  ，和其他没有  `indexed`  的。有  `indexed`  的参数可以使用 filter，例如同一个 Event，我可以选择只监听从特定 address 发出来的交易。每笔 Log 的资料同样分为两部分：  **Topics**  （长度最多为 4 的 array） 和  **Data**  。有 indexed 的参数会储存在 Log 的 Topics，其他的存在 Data。如果宣告为  `anonymous`  ，就不会产生以下范例中的 Topics[0]，其值为 Event signature 的 hash，作为这个 Event 的 ID。

#### 例子
```
pragma solidity ^0.4.20;

contract SimpleStorage {
    uint public data;

    event Set(address indexed _from, uint value);

    function set(uint x) public {
        data = x;
        Set(msg.sender, x);
    }
}
```
智能合约 ABI：

```json
[{
        "constant": true,
        "inputs": [],
        "name": "data",
        "outputs": [{"name": "","type": "uint256"}],
        "payable": false,
        "stateMutabㄒility": "view",
        "type": "function"
    },
    {
        "anonymous": false,
        "inputs": [{"indexed": true,"name": "_from","type": "address"},{"indexed": false,"name": "value","type": "uint256"}],
        "name": "Set",
        "type": "event"
    },
    {
        "constant": false,
        "inputs": [{"name": "x","type": "uint256"}],
        "name": "set",
        "outputs": [],
        "payable": false,
        "stateMutability": "nonpayable",
        "type": "function"
}]
```

参考： http://blockgeek.org/t/topic/394