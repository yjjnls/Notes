[API](http://ericssonresearch.github.io/openwebrtc/docs/gtk-doc/dev/index.html)

    GObject
    ╰── OwrTransportAgent
    ╰── OwrSession
        ├── OwrDataSession
        ╰── OwrMediaSession
    ╰── OwrMediaSource
        ├── OwrLocalMediaSource
        ╰── OwrRemoteMediaSource
    ╰── OwrCandidate

```cpp
void owr_transport_agent_init(OwrTransportAgent *transport_agent){
创建pipeline监听消息
    pipeline_name = g_strdup_printf("transport-agent-%u", priv->agent_id);
    priv->pipeline = gst_pipeline_new(pipeline_name);

创建一个bin，并关联信号
    priv->transport_bin_name = g_strdup_printf("transport_bin_%u", priv->agent_id);
    priv->transport_bin = gst_bin_new(priv->transport_bin_name);

    g_signal_connect(priv->transport_bin, "pad-added", G_CALLBACK(on_transport_bin_pad_added), transport_agent);
    priv->rtpbin = gst_element_factory_make("rtpbin", "rtpbin");
    }
```