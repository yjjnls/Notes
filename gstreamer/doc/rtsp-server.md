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
* create_client (server);--->default_create_client()
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
                            ╰──>[rtsp-media]gst_rtsp_media_collect_streams()
                                ╰──>[rtsp-media]gst_rtsp_media_create_stream()/*create #rtspstream*/
--->[rtsp-media]gst_rtsp_media_prepare()/*The pipeline of the media is set to PAUSED*/
                | //status
                |    GST_RTSP_MEDIA_STATUS_PREPARING
                ╰──>[rtsp-media]default_prepare()
                    ╰──>[rtsp-media]create_rtpbin()/*create #rtpbin*/
                    ╰──>[rtsp-media]start_prepare()
                        ╰──>[rtsp-media]gst_rtsp_stream_join_bin()/*join stream to #rtpbin*/
                        ╰──>[rtsp-media]start_preroll()/*set pipeline GST_STATE_PAUSED*/ 
                            ╰──>[rtsp-media]media_streams_set_blocked()/* start blocked  to make sure nothing goes to the sink */
                            ╰──>[rtsp-media]set_state (media, GST_STATE_PLAYING);
                //signal
                    SIGNAL_PREPARED



typedef enum {
  GST_RTSP_MEDIA_STATUS_UNPREPARED  = 0,
  GST_RTSP_MEDIA_STATUS_UNPREPARING = 1,
  GST_RTSP_MEDIA_STATUS_PREPARING   = 2,
  GST_RTSP_MEDIA_STATUS_PREPARED    = 3,
  GST_RTSP_MEDIA_STATUS_SUSPENDED   = 4,
  GST_RTSP_MEDIA_STATUS_ERROR       = 5
} GstRTSPMediaStatus;

```