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