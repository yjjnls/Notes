# Probe
在pad上插入探针，可以由相应的事件触发回调，然后在回调中处理探针上的数据。  
每个探针类型都表示了一种触发回调的情况（比如有buffer、event等）。

## Blocking Types
阻塞探针触发时会阻塞pad，与该探针类型对应的数据不会继续流动，除非把探针移除或者在回调中返回GST_PAD_PROBE_PASS；而其他不相关的数据会继续流动。

如果在pipeline状态为PLAYING时要添加或者替换元素，不能直接用link、unlink等操作，因为已有的pipeline上可能正好在处理码流。尤其是在删除元素时，需要先在要处理的element的pad上监听IDLE事件，当该element处理完一个buffer时会触发该事件，在buffer的间隙来删除后续元素。

[ref](https://github.com/yjjnls/gst-snippets/blob/master/dynamic-tee-vsink.c)

https://github.com/sdroege/gst-snippets/blob/217ae015aaddfe3f7aa66ffc936ce93401fca04e/dynamic-tee-vsink.c