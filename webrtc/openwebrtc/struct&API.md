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

/*
创建transport_bin，并关联信号
--->on_transport_bin_pad_added()
*/
    priv->transport_bin_name = g_strdup_printf("transport_bin_%u", priv->agent_id);
    priv->transport_bin = gst_bin_new(priv->transport_bin_name);
    g_signal_connect(priv->transport_bin, "pad-added", G_CALLBACK(on_transport_bin_pad_added), transport_agent);
/*
创建一个element：“rtpbin”，并关联信号 
--->on_rtpbin_pad_added()
*/
    priv->rtpbin = gst_element_factory_make("rtpbin", "rtpbin");
    g_signal_connect(priv->rtpbin, "pad-added", G_CALLBACK(on_rtpbin_pad_added), transport_agent);
/*
pipeline包含transport_bin,transport_bin包含rtpbin
pipeline
╰──transport_bin
    ╰──rtpbin
*/
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
    create bin---->receive_output_bin
    create depay-->rtpdepay
    rtpdepay, parser, videorepair1, decoder

```

test-client
```c
owr_init(NULL);
owr_get_capture_sources(OWR_MEDIA_TYPE_AUDIO | OWR_MEDIA_TYPE_VIDEO,
        (OwrCaptureSourcesCallback)got_local_sources, url);
owr_run();
```

```java
owr_get_capture_sources 在owr_loacl模块中设置本地源  
  
owr_get_capture_sources (test_client.c)
╰───owr_transport_agent_new (owr_transport_agent)
    ╰ ─ ─ ─ owr_transport_agent_init (owr_transport_agent)

handle_offer (test_client.c)
|//创建media_session
╰───media_session = owr_media_session_new (transport/owr_media_session)
|//创建并设置send & receive payload
╰───send_payload = receive_payload = owr_video_payload_new()
╰───owr_media_session_add_receive_payload(media_session, receive_payload);
╰───owr_media_session_set_send_payload(media_session, send_payload); 
|//添加remote candidate（如果offer中有）  
╰───owr_session_add_remote_candidate
|//添加remote_candidate/remote source/local candidate/dtls certificate
╰───g_signal_connect(media_session, "on-incoming-source", G_CALLBACK(got_remote_source), NULL);
|//依据local source 设置要发送的source
╰───owr_media_session_set_send_source() 
|//将media_session添加到transport_agent中
╰───owr_transport_agent_add_session(transport_agent, OWR_SESSION(media_session));
```