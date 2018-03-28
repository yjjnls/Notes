## describe

    >> DESCRIBE rtsp://thread:5454/south-rtsp.mp3 RTSP/1.0
    >> Accept: application/sdp
    >> CSeq: 0
    >>
    << RTSP/1.0 200 OK
    << Content-Length: 84
    << Content-Type: application/sdp
    << CSeq: 0
    << Date: Wed May 11 13:09:37 2005 GMT
    <<
    << v=0
    << o=- 0 0 IN IP4 192.168.1.1
    << s=No Title
    << m=audio 0 RTP/AVP 14
    << a=control:streamid=0

> m=audio 0 RTP/AVP 14  

 传输音频，端口号0，用rtp/udp传输，负载净荷类型14

在rtspsrc中首先创建一个udp element（具有随机的偶数端口）来处理来自`udp://0.0.0.0:0 uri`的udp包
一般udp端口+1为rtcp端口

    +-----------------+
    | +------------+  |
    | | udpsrc0    |  |
    | |  port=5000 |  |
    | +------------+  |
    | +------------+  |
    | | udpsrc1    |  |
    | |  port=5001 |  |
    | +------------+  |
    +-----------------+

## setup
    >> SETUP rtsp://thread:5454/south-rtsp.mp3/streamid=0 RTSP/1.0
    >> CSeq: 1
    >> Transport: RTP/AVP/UDP;unicast;client_port=5000-5001,RTP/AVP/UDP;multicast,RTP/AVP/TCP
    >>
    << RTSP/1.0 200 OK
    << Transport: RTP/AVP/UDP;unicast;client_port=5000-5001;server_port=6000-6001
    << CSeq: 1
    << Date: Wed May 11 13:21:43 2005 GMT
    << Session: 5d5cb94413288ccd

客户端要把支持的传输协议、客户端端口号发给服务端。服务端选择最终的协议和端口号，以及服务端自己的端口号。 

在describe中的sdp中服务端先指出支持`RTP/AVP(stand for RTP A/V Profile)`，但是没有具体指定是tcp还是udp  
在setup中，客户端给出支持的tansport（可以有多种），服务端最终决定是用哪一个。  


客户端支持的transport协议可以有多个，用`,`分隔，例如上述为
>RTP/AVP/UDP;unicast;client_port=5000-5001
在5000-5001端口接收rtp/udp（也可以写成默认的RTP/AVP. 即RTP/AVP/UDP）
>RTP/AVP/UDP;multicast
可以加入多播组
>RTP/AVP/TCP
可以在原有rtsp session中用tcp接收udp

服务端要选择第一个支持的transport

    +---------------------------------------------+
    | +------------+                              |
    | | udpsrc0    |   +--------+                 |
    | |  port=5000 ----- rtpdec --------------------
    | +------------+   |        |                 |
    | +------------+   |        |  +------------+ |
    | | udpsrc1    ----- RTCP   ---- udpsink    | |
    | |  port=5001 |   +--------+  |  port=6001 | |
    | +------------+               +------------+ |
    +---------------------------------------------+

## play
    >> PLAY rtsp://thread:5454/south-rtsp.mp3 RTSP/1.0
    >> CSeq: 2
    >> Session: 5d5cb94413288ccd
    >>
    << RTSP/1.0 200 OK
    << CSeq: 2
    << Date: Wed May 11 13:21:43 2005 GMT
    << Session: 5d5cb94413288ccd




```c


gst_rtspsrc_stream_configure_manager()
    gst_rtsp_transport_get_manager (transport->trans, &manager, 0);//获取manager的名字，这里为rtpbin
    src->manager = gst_element_factory_make (manager, "manager");
    ...
    name = g_strdup_printf ("recv_rtp_sink_%u", stream->id);
    stream->channelpad[0] = gst_element_get_request_pad (src->manager, name);
    g_free (name);


gst_rtspsrc_open()
gst_rtspsrc_open_from_sdp()
gst_rtspsrc_setup_streams_start()
gst_rtspsrc_prepare_transports()
gst_rtspsrc_alloc_udp_ports()
    udpsrc0 = gst_element_make_from_uri (GST_URI_SRC, host, NULL, NULL);
    ...
    stream->udpsrc[0] = gst_object_ref_sink (udpsrc0);

gst_rtspsrc_open_from_sdp()
gst_rtspsrc_setup_streams_start()
gst_rtsp_src_setup_stream_from_response()
gst_rtspsrc_stream_configure_transport()
gst_rtspsrc_stream_configure_udp()
    gst_bin_add (GST_BIN_CAST (src), stream->udpsrc[0]);
    *outpad = gst_element_get_static_pad (stream->udpsrc[0], "src");
    gst_pad_link_full (*outpad, stream->channelpad[0], GST_PAD_LINK_CHECK_NOTHING);


    数据从udpsrc出来，传到了rtpbin里面
```