# 以太坊
以太坊通过建立终极的抽象的基础层-`内置有图灵完备编程语言的区块链`-使得任何人都能够**创建合约和去中心化应用**并在其中设立他们自由定义的所有权规则、交易方式和状态转换函数。  

## 以太坊账户
以太坊的账户包含四个部分：
* 随机数，用于确定每笔交易只能被处理一次的计数器
* 账户目前的以太币余额
* 账户的合约代码，如果有的话
* 账户的存储（默认为空）

一般而言，以太坊有两种类型的账户：`外部账户`（由私钥控制的）和`合约账户`（由合约代码控制）。外部所有的账户没有代码，人们可以通过创建和签名一笔`交易`从一个外部账户发送`消息`。合约账户像普通通账户一样也有一个钱包地址，**每当合约账户收到一条消息，合约内部的代码就会被激活**，允许它对内部存储进行读取和写入，和发送其它消息或者创建合约。

* 能且只能以区块链作为输入和输出，因此计算是可重复的 —— 实际上计算的结果并不需要被存储到区块链，因为随时可以重新进行计算。
* 可以调用其他 Contract 中的函数（其他 Contract 的代码和数据同样存在于区块链上）。
* 执行过程中可以创建新的交易（操纵自己的钱包余额），这些交易可能会去执行其他的 Contract.

**合约与外部账户拥有同样权利，包括发送消息和创建其它合约的权利。**

## 交易与消息
### 消息
以太坊的`消息`类似于bitcoin的交易，但是有以下不同：  

*  以太坊的消息可以由外部实体或者合约创建，然而比特币的交易只能从外部创建。
*  以太坊消息可以选择包含数据。
*  如果以太坊消息的接受者是合约账户，可以选择进行回应，这意味着以太坊消息也包含函数概念。

### 交易
以太坊中`交易`是指存储从外部账户发出的消息的签名数据包。交易包含:    
* 消息的接收者
* 用于确认发送者的签名
* 以太币账户余额
* 要发送的数据
* 两个被称为STARTGAS和GASPRICE的数值

solidity需要安装？编译？
evm和etherrum客户端什么关系？

## 区块机制
>当一个矿工 A 挖到一个新的块的时候，他会将这个 Block 广播出去，其他人一旦收到了这个消息，就会立刻基于这个新的块开始工作。而其他人在「A 挖到新的块」和「收到 A 广播的消息」之间这段时间之间的计算实际上是被浪费掉了的，而中心化矿池中的其他矿工则不会有这个问题，因为他们可以更快地得到新产生的块的信息，立刻在新的块的基础上开始工作。   

>这个广播的时间可能需要十几秒，对于 10 分钟来讲这点时间并不是十分重要，但如果去缩短共识间隔，中心化矿池的优势将会越来越明显。但 Ethereum 通过引入「叔块（Uncle Block）」的概念解决了这个问题，将共识间隔减少到了 15 秒钟，在支付确认速度上较 Bitcoin 有了很大的提升。  


>在 Bitcoin 的 Blockchain 中，一个 Block 只能有一个父块，也只能有一个子块。但在 Ethereum 中，一个新产生的块可以有一个父块和若干个叔块。回到上面的例子，**如果在 A 挖到新的块但其他人尚未收到广播的时间中，如果有人挖出了一个新的块，但因为广播较晚没有被大家接受，那么这个块有可能成为下个块的「叔块」—— 这个块所代表的工作量证明会被认为是下一个块的一部分（即这个人挖出下一个块的难度降低了）**，`叔块也仅仅提供工作量证明，其中所包含的交易是无效的`。这样一来便补偿了较晚收到广播的客户端在低出块间隔情况下的劣势，具体来讲，直接的叔块提供 50% 的工作量证明、二代叔块提供 25% 的工作量证明以此类推，最多会承认最多五代的叔块。

question: 区块由谁产生？15秒内没有交易怎么办？

## POS
>所谓 POS 就是说大家用所拥有的 Ether 去做担保，即每一个希望参与 Block 生成（传统意义上的挖矿）的节点（被称为验证人）都需要向系统（这里说的系统是指在协议上做规定，所有节点都认为这笔保证金被「冻结」了）缴纳一笔保证金，然后大家用自己的保证金来对可能成为下一个 Block 的 Block 下注（所谓「可能」的一个重要前提就是这个 Block 必须是符合协议规定的），如果这个块真的成为下一个 Block，那么**所有下注的节点将会得到奖励，否则保证金将会被罚没**。

>当然 POS 之所以目前还未被采用，是因为还存在一些尚未解决的问题，其中之一就是和 POW 一样的 51% 攻击问题，在 POW 中集中全网 51% 的计算力是有一定物理限制的 —— 因为计算力需要计算设备来提供；而相比之下在 POS 中收集全网 51% 的 Ether 则相比之下容易一些 —— 只要你有足够的钱。POS 天然地比 POW 更非复杂，要实现上述的工作逻辑，需要处理例如维护有效的验证人列表、保证金的冻结、罚没和返还、提议区块和投注区块、防止验证人之间的结盟攻击、网络分区之后的恢复等等。

question: pos不用猜数字么？那怎么确定最终哪些人下注的区块是下一个有效的？

## 智能合约（smart contract）
合约位于以太坊区块链上的一个特殊地址。