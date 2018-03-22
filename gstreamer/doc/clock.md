# clock & synchronization

## clock running time
A GstClock returns the absolute-time according to that clock with gst_clock_get_time (). The absolute-time (or clock time) of a clock is monotonically increasing. From the absolute-time is a running-time calculated, which is simply the difference between a previous snapshot of the absolute-time called the base-time. So:  

    running-time = absolute-time - base-time  

**A GStreamer GstPipeline object maintains a GstClock object and a base-time when it goes to the PLAYING state.**   

Because all objects in the pipeline have the same clock and base-time, they can thus all calculate the running-time according to the pipeline clock.   

## Clock providers
When the pipeline goes to the PLAYING state, it will go over all elements in the pipeline from sink to source and ask each element if they can provide a clock. The last element that can provide a clock will be used as the clock provider in the pipeline.   

There exist some bus messages to let you know about the clock and clock providers in the pipeline. You can see what clock is selected in the pipeline by looking at the NEW_CLOCK message on the bus. When a clock provider is removed from the pipeline, a CLOCK_LOST message is posted and the application should go to PAUSED and back to PLAYING to select a new clock.    

## Latency
Since the timestamp of the buffer is 0 and the time of the clock is now >= 1 second, the sink will drop this buffer because it is too late. **Without any latency compensation in the sink, all buffers will be dropped.**

### Latency compensation
Before the pipeline goes to the PLAYING state, it will, in addition to selecting a clock and calculating a base-time, calculate the latency in the pipeline. It does this by doing a LATENCY query on all the sinks in the pipeline. **The pipeline then selects the maximum latency in the pipeline and configures this with a LATENCY event.**

All sink elements will delay playback by the value in the LATENCY event. Since all sinks delay with the same amount of time, they will be relative in sync.

### Dynamic Latency
Adding/removing elements to/from a pipeline or changing element properties can change the latency in a pipeline. An element can request a latency change in the pipeline by posting a LATENCY message on the bus. The application can then decide to query and redistribute a new latency or not. **Changing the latency in a pipeline might cause visual or audible glitches and should therefore only be done by the application when it is allowed.**   


1.  clock time(absolute_time): 管道维护的一个全局时钟，是一个以纳秒为单位单调递增的时钟时间。可以通过gst_clock_get_time()函数获取。如果管道中没有元素提供时钟，则使用该系统时钟。
2.  base time: 媒体从0开始时全局的时间值。可以通过_get_time()函数获取。
3.  Running time: 媒体处于PLAYING状态时流逝的时间。
4.  stream time: 媒体播放的位置（在整个媒体流中）。
公式：   
running_time = clock_time - base_time;   