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

### create sinkpad
```c
static GstStaticPadTemplate sink_template = GST_STATIC_PAD_TEMPLATE ("sink",
    GST_PAD_SINK,
    GST_PAD_ALWAYS,
    GST_STATIC_CAPS_ANY);
self->sinkpad = gst_pad_new_from_static_template (&sink_template, "sink");
gst_element_add_pad (GST_ELEMENT (self), self->sinkpad);
```
sinkpad是整个proxysink的sink pad，真正用于数据传递的是proxysrc。   
**buffer和event先流到sinkpad，然后通过回调gst_proxy_sink_sink_chain，来调用proxysrc传递到proxysink。**
```c
gst_pad_set_chain_function (self->sinkpad, GST_DEBUG_FUNCPTR (gst_proxy_sink_sink_chain));
```

### get sinkpad
```c
return gst_object_ref (self->sinkpad);
```

### link with proxysrc
```c
g_weak_ref_set (&self->proxysrc, src);
```

### push buffer/event to proxysrc
```c
GstProxySrc *src = g_weak_ref_get (&self->proxysrc);
// 获取proxysrc的internal_srcpad
GstPad *srcpad = gst_proxy_src_get_internal_srcpad (src);
gst_pad_push (srcpad, buffer);
// 同时还要将pad上的sticky events 拷贝过去


// push buffer list/event
gst_pad_push_list (srcpad, list);
gst_pad_push_event (srcpad, event);
```
-----

## proxysrc
```c
struct _GstProxySrc {
  GstBin parent;

  /* < private > */

  /* Queue to hold buffers from proxysink */
  GstElement *queue;

  /* Source pad of the above queue and the proxysrc element itself */
  GstPad *srcpad;

  /* Our internal srcpad that proxysink pushes buffers/events/queries into */
  GstPad *internal_srcpad;

  /* An unlinked dummy sinkpad; see gst_proxy_src_init() */
  GstPad *dummy_sinkpad;

  /* The matching proxysink; queries and events are sent to its sinkpad */
  GWeakRef proxysink;
};
```
### create srcpad
```c
static GstStaticPadTemplate src_template = GST_STATIC_PAD_TEMPLATE ("src",
    GST_PAD_SRC,
    GST_PAD_ALWAYS,
    GST_STATIC_CAPS_ANY);
templ = gst_static_pad_template_get (&src_template);
self->srcpad = gst_ghost_pad_new_from_template ("src", srcpad, templ);
gst_element_add_pad (GST_ELEMENT (self), self->srcpad);
```
这个srcpad是整个proxysrc的srcpad，用于link，并没有其他作用。

### create internal srcpad
```c
self->dummy_sinkpad = gst_pad_new ("dummy_sinkpad", GST_PAD_SINK);
gst_object_set_parent (GST_OBJECT (self->dummy_sinkpad), GST_OBJECT (self));

self->internal_srcpad = gst_pad_new ("internal_src", GST_PAD_SRC);
gst_object_set_parent (GST_OBJECT (self->internal_srcpad),
    GST_OBJECT (self->dummy_sinkpad));

sinkpad = gst_element_get_static_pad (self->queue, "sink");
gst_pad_link (self->internal_srcpad, sinkpad);
gst_object_unref (sinkpad);
```
internal_srcpad的parent是dummy_sinkpad，dummy_sinkpad的parent是self bin，这样就可以把internal_srcpad和queue连起来。   
而internal_srcpad是和proxysink连起来的。

### push event to upstream
这里用GWeakRef proxysink来**反向push事件**，GWeakRef proxysink是对整个proxysink的weak_ref，通过获取proxysink的sinkpad直接推送数据。

