element为pipeline的最小组成单位，包含多个pads（source/sink）   
多个element连接组成bin（高级功能单元）  
特殊属性的bin为pipeline（包含全局时钟+gst-bus）  

## 1.	create pipeline/bin
GstElement* `gst_pipeline_new`(const gchar *name);

GstStateChangeReturn `gst_element_set_state`(GstElement *element, GstState state);  
void	 `gst_object_unref`(gpointer object);
```cpp
GstElement *pipeline_ = gst_pipeline_new(“xxx”);
gst_element_set_state(pipeline, GST_STATE_NULL);
gst_object_unref(pipeline);
```

## 2.	bus(listen to message) 
GstBus* `gst_pipeline_get_bus`(GstPipeline *pipeline);  
guint `gst_bus_add_watch`(GstBus * bus, GstBusFunc func, gpointer user_data);

gboolean `g_source_remov`(guint tag);  
void	 `gst_object_unref`(gpointer object);
```cpp
GstBus *bus_ = gst_pipeline_get_bus(GST_PIPELINE(pipeline_));
guint bus_watcher_ = gst_bus_add_watch(bus_, MessageHandler_, this);
g_source_remove(bus_watcher_);
gst_object_unref(bus_);
```

## 3.	create element
`gst_element_factory_make`  
```cpp
GstElement *rtspsrc = gst_element_factory_make("rtspsrc", "src");
```
第一个参数是element的类型，第二个参数是element的名字  
还可以用g_object_set来设置element的属性  


An element can be retrieved from a bin with `gst_bin_get_by_name()`, using the elements name. It gets the element with the given name from a bin. This function **recurses into child bins**. `gst_bin_get_by_name_recurse_up()` is mainly used for internal purposes and will **query the parent bins** when the element is not found in the current bin.
```cpp
g_object_set(G_OBJECT(rtspsrc), "location", data.url.c_str(), NULL);
```


## 4.	add/remove element
gboolean `gst_bin_add`(GstBin *bin, GstElement *element);  
void `gst_bin_add_many`(GstBin *bin, GstElement *element_1, ...);  
gboolean `gst_bin_remove`(GstBin *bin, GstElement *element);  


The “element-added” signal is fired whenever a new element is added to the bin. Likewise the “element-removed” signal is fired whenever an element is removed from the bin. [ref](https://gstreamer.freedesktop.org/data/doc/gstreamer/head/gstreamer/html/GstBin.html)

## 5.	link element
连接element的过程就是将相邻element的sink pad和source pad连接起来
Gboolean `gst_pad_can_link`(GstPad *srcpad, GstPad *sinkpad);  
GstPadLinkReturn `gst_pad_link`(GstPad *srcpad, GstPad *sinkpad); 
```cpp
gst_bin_add_many(GST_BIN(owner.pipeline()), rtspsrc, rtph264depay, h264parse, NULL);
g_signal_connect(rtspsrc, "pad-added", (GCallback)on_rtspsrc_pad_added, this);
g_warn_if_fail(gst_element_link(rtph264depay, h264parse));
```
某些element需要动态分配pad，所以为其添加"pad-added"信号，然后在回调函数中继续处理link  

GstPad *`gst_element_get_static_pad`(GstElement *element, const gchar *name);  

```cpp
void on_rtspsrc_pad_added(GstElement* src, GstPad* src_pad, gpointer rtspclient)
{
    RtspClient* rtsp_client = static_cast<RtspClient*>(rtspclient);
    GstCaps* caps = gst_pad_query_caps(src_pad, NULL);
    GstStructure *stru = gst_caps_get_structure(caps, 0);
    const GValue* media_type = gst_structure_get_value(stru, "media");

    if (g_str_equal(g_value_get_string(media_type), "video"))
    {
        GstPad* sink_pad = gst_element_get_static_pad(GST_ELEMENT_CAST(rtsp_client->rtph264depay), "sink");
        GstPadLinkReturn ret = gst_pad_link(src_pad, sink_pad);
        g_warn_if_fail(ret == GST_PAD_LINK_OK);
        gst_object_unref(sink_pad);
    }
}
```
caps为element能处理的类型,兼容类型才能link  
GstCaps *`gst_pad_get_current_caps`(GstPad * pad);  
gboolean	 `gst_pad_has_current_caps`(GstPad * pad);
## 6.	signal
>#define g_signal_connect(instance, detailed_signal, c_handler, data) \
>g_signal_connect_data ((instance), (detailed_signal), (c_handler), (data), NULL, (GConnectFlags) 0)

g_signal_new  
g_signal_emit
