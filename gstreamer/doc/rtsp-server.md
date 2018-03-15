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

```