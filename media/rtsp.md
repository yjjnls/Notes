# RTSP (real time streaming protocol)

## GET_PARAMETER

GET_PARAMETER请求检查URL指定的演示与媒体的参数值。  
**没有实体体时，GET_PARAMETER也许能用来测试用户与服务器的连通情况**

    OPTIONS rtsp://172.16.66.68/id=1 RTSP/1.0
    CSeq: 1
    User-Agent: GStreamer/1.12.2
    User-Agent: RealMedia Player Version 6.0.9.1235 (linux-2.0-libc6-i386-gcc2.95)
    ClientChallenge: 9e26d33f2984236010ef6253fb1887f7
    CompanyID: KnKV4M4I/B2FjJ1TToLycw==
    GUID: 00000000-0000-0000-0000-000000000000
    RegionData: 0
    PlayerStarttime: [28/03/2003:22:50:23 00:00]
    ClientID: Linux_2.4_6.0.9.1235_play32_RN01_EN_586
    Date: Wed, 29 Nov 2017 08:08:21 GMT

    RTSP/1.0 200 OK
    CSeq: 1
    Public: OPTIONS, DESCRIBE, SETUP, PLAY, PAUSE, TEARDOWN,GET_PARAMETER, SET_PARAMETER

    DESCRIBE rtsp://172.16.66.68/id=1 RTSP/1.0
    CSeq: 2
    User-Agent: GStreamer/1.12.2
    Accept: application/sdp
    Date: Wed, 29 Nov 2017 08:08:21 GMT

    RTSP/1.0 200 OK
    CSeq: 2
    Content-Type: application/sdp
    Content-Length: 360

    v=0
    o=NVR 1511942897 1511942897 IN IP4 0.0.0.0
    s=3GPP Unicast SDP 
    c=IN IP4 0.0.0.0
    t=0 0
    a=range:npt=0-
    a=control:*
    m=video 0 RTP/AVP 96
    a=control:video_0
    a=rtpmap:96 H264/90000
    a=fmtp:96 profile-level-id=42001e;sprop-parameter-sets=Z0IAHp2oLA9puAgICBA=,aM48gA==;packetization-mode=0
    m=audio 0 RTP/AVP 8
    a=control:audio_0
    a=rtpmap:8 PCMA/8000

    SETUP rtsp://172.16.66.68/id=1/video_0 RTSP/1.0
    CSeq: 3
    User-Agent: GStreamer/1.12.2
    Transport: RTP/AVP;unicast;client_port=52574-52575
    Date: Wed, 29 Nov 2017 08:08:21 GMT

    RTSP/1.0 200 OK
    CSeq: 3
    Session: 1511942897;timeout=60
    Transport: RTP/AVP;unicast;mode=play;client_port=52574-52575;server_port=56228-56229
    Cache-Control: no-cache

    SETUP rtsp://172.16.66.68/id=1/audio_0 RTSP/1.0
    CSeq: 4
    User-Agent: GStreamer/1.12.2
    Transport: RTP/AVP;unicast;client_port=52576-52577
    Session: 1511942897
    Date: Wed, 29 Nov 2017 08:08:21 GMT

    RTSP/1.0 455 Method Not Valid In This State
    CSeq: 4
    Session: 1511942897;timeout=60
    Transport: RTP/AVP;unicast;mode=play;client_port=52576-52577;server_port=0-0
    Cache-Control: no-cache

    GET_PARAMETER rtsp://172.16.66.68/id=1 RTSP/1.0
    CSeq: 5
    User-Agent: GStreamer/1.12.2
    Session: 1511942897
    Date: Wed, 29 Nov 2017 08:09:17 GMT

    RTSP/1.0 200 OK
    CSeq: 5
    Session: 1511942897

    GET_PARAMETER rtsp://172.16.66.68/id=1 RTSP/1.0
    CSeq: 6
    User-Agent: GStreamer/1.12.2
    Session: 1511942897
    Date: Wed, 29 Nov 2017 08:10:13 GMT

    RTSP/1.0 200 OK
    CSeq: 6
    Session: 1511942897

## 出现异常

rtsp获取码流时，如果在某一步出错了，那么从response就能看出

    DESCRIBE rtsp://172.16.66.66/id=33 RTSP/1.0
    CSeq: 2
    User-Agent: GStreamer/1.12.2
    Accept: application/sdp
    Date: Wed, 29 Nov 2017 08:29:39 GMT

    RTSP/1.0 404 Not Found
    CSeq: 2
    Date: Wed, Nov 29 2017 08:28:06 GMT

    PLAY rtsp://172.16.66.68/id=1 RTSP/1.0
    CSeq: 5
    User-Agent: GStreamer/1.12.2
    Range: npt=0-
    Session: 1511936242 
    Date: Wed, 29 Nov 2017 06:17:26 GMT

    RTSP/1.0 400 Bad Request
    CSeq: 5
    Session: 1511936242
    RTP-Info: url=video/id=0;seq=0;rtptime=0,url=audio/id=0;seq=0;rtptime=0
    Range: npt=0-
    Date: Wed, Dec 29 2017 06:17:22 GMT

    GET_PARAMETER rtsp://172.16.66.68/id=1 RTSP/1.0
    CSeq: 6
    User-Agent: GStreamer/1.12.2
    Session: 1511936242
    Date: Wed, 29 Nov 2017 06:18:21 GMT

    RTSP/1.0 200 OK
    CSeq: 6
    Session: 1511936242

### 几种异常

1.  unauthorized
    describe时返回401

2.  url不存在
    无法建立tcp连接，在tcp层面报错
    在用tcp传输码流时其实并不需要rtcp，只需要判断tcp是否断链

3.  profile错误
    describe 404

## Header filed definitions

url=<URL>;[seq=<seqbase>;rtptime=<timebase>] [, url=...]

```
RTSP/1.0 200 OK
CSeq: 5
Session: 1514044875
RTP-Info: url=video/id=0;seq=0;rtptime=0,url=audio/id=0;seq=0;rtptime=0
Range: npt=0-
Date: Sat, Dec 23 2017 16:01:15 GMT
```

seq这个值是在seek之后的第一个序列号
如果码流能seek的话那应该url和setup中的一样


### 快进
再发个PLAY命令,包含scale字段,服务端对其作出响应

### 服务端检测客户端是否在线
1.  客户端定时发送get_paramter或者option进行心跳。这时候得依赖于客户端的行为。
2.  如果客户端没有该功能，则服务端可以根据客户端发来的rtcp信息来作为心跳检测。
    但是setup中协商的是客户端接收rtp和rtcp的端口，并不知道客户端发送rtcp的端口。
    一般客户端使用同一个端口收发rtcp。

http://gstreamer-devel.966125.n4.nabble.com/Timeout-in-rtspsrc-td3274173.html

### 传输方式设定

```
RTSP request message 00000088A8FFF660
 request line:
   method: 'SETUP'
   uri:    'rtsp://172.16.66.65/id=1/video_0'
   version: '1.0'
 headers:
   key: 'User-Agent', value: 'GStreamer/1.12.3'
   key: 'Transport', value: 'RTP/AVP/TCP;unicast;interleaved=0-1'
 body:
RTSP response message 00000088A8FFF6C0
 status line:
   code:   '200'
   reason: 'OK'
   version: '1.0'
 headers:
   key: 'CSeq', value: '3'
   key: 'Session', value: '1520240922;timeout=60'
   key: 'Transport', value: 'RTP/AVP/TCP;interleaved=0-1'
   key: 'Cache-Control', value: 'no-cache'
 body: length 0
 ```

 SETUP:定义RTSP客户端和RTSP服务器之间实时流的传输方式。传输方式可以是TCP、UDP或者是MULTICAST其中之一。通过在SETUP发法中定义Transport Header的字段，可以定义RTSP客户端和RTSP服务器端使用什么方式进行流传输。Transport Header定义如下：
      `Transport: transport-protocol/profile/lower-transport/parameter`
      transport-protocol = 传输协议，一般填写RTP
      profile = 概述，固定填写AVP(Audio Video Profile)
      **lower-transport = 底层传输协议，可以填写TCP或者UDP。如果不填写，则默认使用UDP作为底层传输协议。**
      Parameter = transport-protocol/profile[/lower-transport] ( ”unicast” | ”multicast” )
      | ”;” ”interleaved” ”=” channel [ ”-” channel ]
      | ”;” ”port” ”=” port [ ”-” port ]
      | ”;” ”client port” ”=” port [ ”-” port ]
      | ”;” ”server port” ”=” port [ ”-” port ]
      | ”;” ”mode” = <”> 1#mode <”>
      unicast = 单播，multicast = 多播，该参数为必填参数
      interleaved = 使用传输流和RTSP控制混合传输的方式
      port = 多播传输时的端口
      client port = UDP做为底层传输协议时的RTSP客户端的端口号
      server port = 使用UDP做为底层传输协议时的RTSP服务器的端口号
      mode = 即将进行的取流操作。可以填写PLAY或者RECORD。

## 双向对讲
http://blog.csdn.net/yuanchunsi/article/details/72877603