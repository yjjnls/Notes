evutil_socket_t  普通socket
	evutil_socketpair
	evutil_closesocket

bufferevent  包含两个evbuffer，两个event，用与io读写
	bufferevent_socket_new  绑定socket，socket收发消息都会存在bufferevent中
	bufferevent_free
		bufferevent_setcb
		bufferevent_enable

event_base
	event_base_new
	event_base_free
		event_base_dispatch
		event_base_loopbreak 要先将base中的socket都关闭，再调用这个

evhttp
	evhttp_new
	evhttp_free
		evhttp_set_cb
		evhttp_bind_socket

evbuffer  单个缓冲区
	evbuffer_new
	evbuffer_free
		evbuffer_add  向buffer中添加内容
		evbuffer_get_length 读取request中的内容时用的
		
evhttp_request  
	evhttp_request_new
		evhttp_request_get_input_buffer 读取request中的内容时用的 （返回evbuffer *）
		evhttp_request_get_output_buffer 发送request前获取其指针，并添加内容（返回evbuffer*）
		evhttp_make_request  发送请求
		evhttp_cancel_request
		evhttp_send_reply
evhttp_connection
	evhttp_connection_base_new
	evhttp_connection_free