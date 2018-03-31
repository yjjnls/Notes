# webrtc-sendrecv analysis

1.  启动程序连接signal bridge，消息接口on_server_message
2.  接收到HELLO消息，setup_call与对端连接
3.  接收到SESSION_OK消息，start_pipeline，创建webrtc pipeline
    包括create offer、set local description、send offer 和send ice candidate。
    offer sdp会根据pipeline中的caps来自动生成。
4.  接收到answer sdp消息，解析并set remote description
5.  接收到candidate，set remote candidate