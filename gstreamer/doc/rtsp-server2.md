# rtsp-server 源码分析
```c
[rtsp-server]
gst_rtsp_server_attach (GstRTSPServer * server, GMainContext * context)
//create a source from server and attach the source to the context
            |
            |
            ↓
gst_rtsp_server_create_source(GstRTSPServer * server, GCancellable * cancellable, GError ** error)
// listening for connections on a port and creating #GstRTSPClient objects to handle those connections.
// When a new connection is received, a new #GstRTSPClient object is created to handle the connection.
* gst_rtsp_server_create_socket()
* g_socket_create_source() /* create a watch for reads (new connections) and possible errors */
* g_source_set_callback()--->gst_rtsp_server_io_func()
                                    ||
                                    ||
                                    ↓↓
gst_rtsp_server_io_func(GSocket * socket, GIOCondition condition, GstRTSPServer * server)
// creates a new #GstRTSPClient to accept and handle a new connection on @socket or @server.
* create_client (server);--->default_create_client()--->[rtsp-client]gst_rtsp_client_new()
* [rtsp-client] gst_rtsp_client_set_connection (client, conn);
   //log
        ** Message: [rtsp-client] client 000001C62478F110 connected to server ip 127.0.0.1, ipv6 = 0
        ** Message: [rtsp-client] added new client 000001C62478F110 ip 127.0.0.1:57323
* manage_client (server, client);/* add the client context to the active list of clients, takes ownership of client */
|  //signal
|       SIGNAL_CLIENT_CONNECTED
|       g_signal_connect (client, "closed", (GCallback) unmanage_client, cctx);
╰──╮
   ↓
gst_rtsp_client_attach()--->gst_rtsp_watch_new()--->watch_funcs()--->message_received()
                                                                 --->message_sent()
                                                                 ...


[rtsp-client]
message_received()--->gst_rtsp_client_handle_message()--->handle_request()

//DESCRIBE
handle_describe_request()
|   //signal
|        SIGNAL_PRE_DESCRIBE_REQUEST
|        SIGNAL_DESCRIBE_REQUEST
╰──╮
   ↓        
find_media()
--->[rtsp-media-factory]gst_rtsp_media_factory_construct()/*create #rtspmedia*/
                        | //signal
                        |    SIGNAL_MEDIA_CONSTRUCTED
                        |    SIGNAL_MEDIA_CONFIGURE
                        ╰──>[rtsp-media-factory]default_construct()
                            ╰──>[rtsp-media-factory]gst_rtsp_media_factory_create_element()/*create #element according launch*/★
                            ╰──>[rtsp-media]gst_rtsp_media_collect_streams()
                                ╰──>[rtsp-media]gst_rtsp_media_create_stream(media, elem, pad)/*create #rtspstream*/★
                            ╰──>[rtsp-media-factory]default_create_pipeline()
                                pipeline = gst_pipeline_new ("media-pipeline");
                                ╰──>[rtsp-media]gst_rtsp_media_take_pipeline(media,pipeline);/*create #rtspstream #pipeline and add #element into #pipeline*/★
--->[rtsp-media]gst_rtsp_media_prepare()/*The pipeline of the media is set to PAUSED*/
                | //status
                |    GST_RTSP_MEDIA_STATUS_PREPARING
                ╰──>[rtsp-media]default_prepare()
                    ╰──>[rtsp-media]create_rtpbin()/*create #rtpbin*/
                    ╰──>[rtsp-media]gst_bin_add (GST_BIN (priv->pipeline), priv->rtpbin);/*add rtpbin to rtpsmedia pipeline*/★
                    ╰──>[rtsp-media]start_prepare()
                        ╰──>[rtsp-stream]gst_rtsp_stream_join_bin()/*join stream to #rtpbin*/
                            ╰──>[rtsp-stream]create_sender_part(stream, bin, state);
                            ╰──>[rtsp-stream]create_receiver_part(stream, bin, state);
                        ╰──>[rtsp-media]start_preroll()/*set pipeline GST_STATE_PAUSED*/ 
                            ╰──>[rtsp-media]media_streams_set_blocked()/* start blocked  to make sure nothing goes to the sink */
                            ╰──>[rtsp-media]set_state (media, GST_STATE_PLAYING);
                //signal
                    SIGNAL_PREPARED

### media->priv->pipeline
包含整条流的pipeline ，可以通过gst_rtsp_media_take_pipeline()设置

### media->priv->element
// create by gst_rtsp_media_new(), it will provide the streaming elements. 
gst_rtsp_media_factory_set_launch()里面的根据语法解析出来的element会添加到[media->priv->element]里面  
media->priv->element可以看作是一个bin

// For each of the streams, a new GstRTSPStream object needs to be made with the gst_rtsp_media_create_stream() 
gst_rtsp_media_create_stream(media, pay, pad)
* pay为media->priv->element中提取出的payload element，也就是"xxxpay"这样的element。  
* pad为elem的src pad

1.  根据pad创建一个ghost_pad并添加到media->priv->element中去。
2.  创建stream（GstElement），`stream会和ghost_pad连接`。 
    stream = gst_rtsp_stream_new (idx, payloader, ghostpad);
    `stream->priv->srcpad = gst_object_ref (ghostpad);`
3.  创建的stream添加到priv->streams这个数组中去。

gst_rtsp_media_take_pipeline()--->gst_bin_add (GST_BIN_CAST (pipeline), priv->element);
gst_rtsp_media_set_property

## rtsp-stream
gst_rtsp_stream_join_bin()
1.  stream的`srcpad`和rtpbin的send_rtp_sink pad连接起来，**也即将element与rtpbin连起来**
2.  create_sender_part (stream, bin, state);
    这个bin就是priv->pipeline，这里会创建tee和用于发rtp/rtcp的udpsink或者appsink，然后把tee和rtpbin连接起来
3.  create_receiver_part (stream, bin, state);


create_sender_part
/* For the sender we create this bit of pipeline for both
* RTP and RTCP. Sync and preroll are enabled on udpsink so
* we need to add a queue before appsink and udpsink to make
* the pipeline not block. For the TCP case, we want to pump
* client as fast as possible anyway. This pipeline is used
* when both TCP and UDP are present.
*
* .--------.      .-----.    .---------.    .---------.
* | rtpbin |      | tee |    |  queue  |    | udpsink |
* |       send->sink   src->sink      src->sink       |
* '--------'      |     |    '---------'    '---------'
*                 |     |    .---------.    .---------.
*                 |     |    |  queue  |    | appsink |
*                 |    src->sink      src->sink       |
*                 '-----'    '---------'    '---------'
*
* When only UDP or only TCP is allowed, we skip the tee and queue
* and link the udpsink (for UDP) or appsink (for TCP) directly to
* the session.
*/

create_receiver_part
/* For the receiver we create this bit of pipeline for both
* RTP and RTCP. We receive RTP/RTCP on appsrc and udpsrc
* and it is all funneled into the rtpbin receive pad.
*
* .--------.     .--------.    .--------.
* | udpsrc |     | funnel |    | rtpbin |
* |       src->sink      src->sink      |
* '--------'     |        |    '--------'
* .--------.     |        |
* | appsrc |     |        |
* |       src->sink       |
* '--------'     '--------'
*/

typedef enum {
  GST_RTSP_MEDIA_STATUS_UNPREPARED  = 0,
  GST_RTSP_MEDIA_STATUS_UNPREPARING = 1,
  GST_RTSP_MEDIA_STATUS_PREPARING   = 2,
  GST_RTSP_MEDIA_STATUS_PREPARED    = 3,
  GST_RTSP_MEDIA_STATUS_SUSPENDED   = 4,
  GST_RTSP_MEDIA_STATUS_ERROR       = 5
} GstRTSPMediaStatus;

```