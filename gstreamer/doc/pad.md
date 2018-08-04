First, you probably want to use RFC 4585 NACK PLI instead of RFC 5104 
FIR (or RFC 2032 FIR, those are only defined for H.261) for this 
use-case. 

To do that with GStreamer: 
1. add a field named "rtcp-fb-nack-pli" to your rtp caps, so if you send 
H264 with PT 96, you'd have something like: 
application/x-rtp, payload=(int)96, encoding-name=(string)H264, 
clock-rate=(int)90000, rtcp-fb-nack-pli=(int)1 

2. Make sure that your decoder sends an upstream GstForceKeyUnit event 
when it detects an image loss (note: gst-libav still doesn't do that). 



```c
if (rtph264depay->sps->len == 0 || rtph264depay->pps->len == 0) {
    /* Down push down any buffer in non-bytestream mode if the SPS/PPS 
    *  haven't go through yet 
    */
    gst_pad_push_event(GST_RTP_BASE_DEPAYLOAD_SINKPAD(depayload),
                       gst_event_new_custom(GST_EVENT_CUSTOM_UPSTREAM,
                                            gst_structure_new("GstForceKeyUnit",
                                                              "all-headers",
                                                              G_TYPE_BOOLEAN,
                                                              TRUE,
                                                              NULL)));
    gst_buffer_unmap(nal, &map);
    gst_buffer_unref(nal);
    return NULL; 
    } 
```


`gst_pad_push_event ()`  
Sends the event to the peer of the given pad. This function is mainly used by elements to send events to their peer elements.

This function takes ownership of the provided event so you should gst_event_ref() it if you want to reuse the event after this call.

`gst_pad_send_event ()`  
Sends the event to the pad. This function can be used by applications to send events in the pipeline.

If pad is a source pad, event should be an upstream event. If pad is a sink pad, event should be a downstream event. For example, you would not send a GST_EVENT_EOS on a src pad; EOS events only propagate downstream. Furthermore, some downstream events have to be serialized with data flow, like EOS, while some can travel out-of-band, like GST_EVENT_FLUSH_START. If the event needs to be serialized with data flow, this function will take the pad's stream lock while calling its event function.

To find out whether an event type is upstream, downstream, or downstream and serialized, see GstEventTypeFlags, gst_event_type_get_flags(), GST_EVENT_IS_UPSTREAM, GST_EVENT_IS_DOWNSTREAM, and GST_EVENT_IS_SERIALIZED. Note that in practice that an application or plugin doesn't need to bother itself with this information; the core handles all necessary locks and checks.

This function takes ownership of the provided event so you should gst_event_ref() it if you want to reuse the event after this call.