## GET_PARAMETER
GET_PARAMETER请求检查URL指定的演示与媒体的参数值。  
**没有实体体时，GET_PARAMETER也许能用来测试用户与服务器的连通情况**

```
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

```

## 出现异常
rtsp获取码流时，如果在某一步出错了，那么从response就能看出

```
DESCRIBE rtsp://172.16.66.66/id=33 RTSP/1.0
CSeq: 2
User-Agent: GStreamer/1.12.2
Accept: application/sdp
Date: Wed, 29 Nov 2017 08:29:39 GMT

RTSP/1.0 404 Not Found
CSeq: 2
Date: Wed, Nov 29 2017 08:28:06 GMT
```

```
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
```