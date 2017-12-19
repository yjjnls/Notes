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
