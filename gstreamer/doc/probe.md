# Probe
在pad上插入探针，可以由相应的事件触发回调，然后在回调中处理探针上的数据。  
每个探针类型都表示了一种触发回调的情况（比如有buffer、event等）。

## Blocking Types
阻塞探针触发时会阻塞pad，与该探针类型对应的数据不会继续流动，除非把探针移除或者在回调中返回GST_PAD_PROBE_PASS；而其他不相关的数据会继续流动。

https://github.com/sdroege/gst-snippets/blob/217ae015aaddfe3f7aa66ffc936ce93401fca04e/dynamic-tee-vsink.c