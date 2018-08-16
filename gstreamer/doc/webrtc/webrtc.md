'gst_dtls_connection_process: runtime check failed: (!priv->bio_buffer)'

>where the encoder needs to be started (and SSL initialized) before the associated decoder receives any data and calls gst_dtls_connection_process().

这种情况可能是本地webrtcbin发送了sdp和candidate，但是没有收到remote sdp和candidate或者还没设置，对方就已经把码流发过来了，也就是decoder先收到了数据调用了`gst_dtls_connection_process`