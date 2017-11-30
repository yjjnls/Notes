## 1.	create pipeline/bin
>GstElement* gst_pipeline_new(const gchar *name);

>GstStateChangeReturn gst_element_set_state(GstElement *element, GstState state);  
>void	 gst_object_unref(gpointer object);
```cpp
GstElement *pipeline_ = gst_pipeline_new(“xxx”);
gst_element_set_state(pipeline, GST_STATE_NULL);
gst_object_unref(pipeline);
```

## 2.	bus(listen to message) 
>GstBus* gst_pipeline_get_bus(GstPipeline *pipeline);  
>guint gst_bus_add_watch(GstBus * bus, GstBusFunc func, gpointer user_data);

>gboolean g_source_remov(guint tag);  
>void	 gst_object_unref(gpointer object);
```cpp
GstBus *bus_ = gst_pipeline_get_bus(GST_PIPELINE(pipeline_));
guint bus_watcher_ = gst_bus_add_watch(bus_, MessageHandler_, this);
g_source_remove(bus_watcher_);
gst_object_unref(bus_);
```

## 3.	create element
## 4.	add/remove element
>gboolean	 gst_bin_add	(GstBin *bin, GstElement *element);  
>gboolean gst_bin_remove(GstBin *bin, GstElement *element);  

## 5.	link element
>Gboolean gst_pad_can_link(GstPad *srcpad, GstPad *sinkpad);  
>GstPadLinkReturn gst_pad_link(GstPad *srcpad, GstPad *sinkpad); 

>//element能处理的类型,兼容类型才能link  
>GstCaps *gst_pad_get_current_caps(GstPad * pad);  
>gboolean	 gst_pad_has_current_caps(GstPad * pad);

## 6.	signal
>#define g_signal_connect(instance, detailed_signal, c_handler, data) \
>g_signal_connect_data ((instance), (detailed_signal), (c_handler), (data), NULL, (GConnectFlags) 0)

g_signal_new
g_signal_emit
