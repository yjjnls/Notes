# LRU

在Cache中找出近期最少使用的信息块，将其替换出去

## 计数器实现

1.  未命中时，被调入的块，计数器清零，其他块计数器加1；
2.  命中时，所有块的计数值与命中块的比，小于命中块则+1，若大于命中块的计数值，则不变；命中块计数器清零。

## 栈实现

栈底表示计数器最大，最久没使用。新元素入栈，若
1\. 未命中，则push到栈顶。
2\. 命中，则将命中元素弹出，push到栈顶。

感觉也可以用链表和哈希表配合实现。  
