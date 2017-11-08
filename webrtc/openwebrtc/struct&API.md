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
// 创建pipeline监听消息
    pipeline_name = g_strdup_printf("transport-agent-%u", priv->agent_id);
    priv->pipeline = gst_pipeline_new(pipeline_name);

// 创建transport_bin，并关联信号 on_transport_bin_pad_added
    priv->transport_bin_name = g_strdup_printf("transport_bin_%u", priv->agent_id);
    priv->transport_bin = gst_bin_new(priv->transport_bin_name);
    g_signal_connect(priv->transport_bin, "pad-added", G_CALLBACK(on_transport_bin_pad_added), transport_agent);

// 创建一个element：“rtpbin”，并关联信号 
    priv->rtpbin = gst_element_factory_make("rtpbin", "rtpbin");
    g_signal_connect(priv->rtpbin, "pad-added", G_CALLBACK(on_rtpbin_pad_added), transport_agent);

// pipeline包含transport_bin,transport_bin包含rtpbin
    gst_bin_add(GST_BIN(priv->transport_bin), priv->rtpbin);
    gst_bin_add(GST_BIN(priv->pipeline), priv->transport_bin);
}
    

// gboolean add_session(GHashTable *args)
//     ->prepare_transport_bin_receive_elements()
//     // 触发 on_rtpbin_pad_added
//         gst_element_link_pads(receive_input_bin, "rtp_src", transport_agent->priv->rtpbin,
//         rtpbin_pad_name);

void on_rtpbin_pad_added(...) 
        |
        |
void setup_video_receive_elements(...)


```