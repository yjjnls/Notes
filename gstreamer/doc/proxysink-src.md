# proxysink & proxysrc

## proxysink
```c
struct _GstProxySink {
  GstElement parent;

  /* < private > */
  GstPad *sinkpad;

  /* The proxysrc that we push events, buffers, queries to */
  GWeakRef proxysrc;

  /* Whether there are sticky events pending */
  gboolean pending_sticky_events;
};
```

* create internal_pad
```c
static GstStaticPadTemplate sink_template = GST_STATIC_PAD_TEMPLATE ("sink",
    GST_PAD_SINK,
    GST_PAD_ALWAYS,
    GST_STATIC_CAPS_ANY);
self->sinkpad = gst_pad_new_from_static_template (&sink_template, "sink");
gst_element_add_pad (GST_ELEMENT (self), self->sinkpad);
```
* 获取internal_pad
```c
return gst_object_ref (self->sinkpad);
```

* 与proxysrc连接
```c
g_weak_ref_set (&self->proxysrc, src);
```

* 推送数据到proxysrc
```c
GstProxySrc *src = g_weak_ref_get (&self->proxysrc);
// 获取proxysrc的internal_pad
GstPad *srcpad = gst_proxy_src_get_internal_srcpad (src);
gst_pad_push (srcpad, buffer);
// 同时还要将pad上的sticky events 拷贝过去


// push buffer list/event
gst_pad_push_list (srcpad, list);
gst_pad_push_event (srcpad, event);
```