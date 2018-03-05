# RTMP (Real Time Message Protocol)
该协议基于 TCP，是一个协议族，包括 RTMP 基本协议及 RTMPT/RTMPS/RTMPE 等多种变种。RTMP 是一种设计用来进行实时数据通信的网络协议，主要用来在 Flash/AIR 平台和支持 RTMP 协议的流媒体/交互服务器之间进行音视频和数据通信。支持该协议的软件包括 Adobe Media Server/Ultrant Media Server/red5 等。

**RTMP协议中，视频必须是H264编码，音频必须是AAC或MP3编码，且多以flv格式封包。**

* 传输层建立完成TCP链接后，应用层还需要通过握手来建立RTMP Connection链接。
* 在RTMP Connection链接中传输媒体数据和信令。
* RTMP协议会对数据做自己的格式化，称为RTMP Message。
* 为了实现多路复用、分包和信息的公平性，发送端会把`Message`划分为带有Message ID的`Chunk`，每个Chunk可能是一个单独的Message，也可能是Message的一部分。

## 握手
要建立一个有效的RTMP Connection链接，首先要“握手”:客户端要向服务器发送C0,C1,C2（按序）三个chunk，服务器向客户端发送S0,S1,S2（按序）三个chunk，然后才能进行有效的信息传输。RTMP协议本身并没有规定这6个Message的具体传输顺序，但RTMP协议的实现者需要保证这几点：
* 客户端要等收到S1之后才能发送C2
* 客户端要等收到S2之后才能发送其他信息（控制信息和真实音视频等数据）
* 服务端要等到收到C0之后发送S1
* 服务端必须等到收到C1之后才能发送S2
* 服务端必须等到收到C2之后才能发送其他信息（控制信息和真实音视频等数据） 

理论上来讲只要满足以上条件，如何安排6个Message的顺序都是可以的，但实际实现中为了在保证握手的身份验证功能的基础上尽量减少通信的次数，一般的发送顺序是这样的，这一点可以通过wireshark抓ffmpeg推流包进行验证： 
｜client｜Server ｜ 
｜－－－C0+C1—->| 
｜<－－S0+S1+S2– | 
｜－－－C2-－－－> ｜       

详细参考
[RTMP H5 直播流技术解析](https://www.villainhr.com/page/2017/08/05/RTMP%20H5%20%E7%9B%B4%E6%92%AD%E6%B5%81%E6%8A%80%E6%9C%AF%E8%A7%A3%E6%9E%90)