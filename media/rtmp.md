# RTMP (Real Time Message Protocol)
该协议基于 TCP，是一个协议族，包括 RTMP 基本协议及 RTMPT/RTMPS/RTMPE 等多种变种。RTMP 是一种设计用来进行实时数据通信的网络协议，主要用来在 Flash/AIR 平台和支持 RTMP 协议的流媒体/交互服务器之间进行音视频和数据通信。支持该协议的软件包括 Adobe Media Server/Ultrant Media Server/red5 等。

## 优缺点
### http
* 一般点播用http就可以满足，hls、http-flv
* CDN支持好，适用于大规模直播，rtmp也有CDN支持，但肯定贵


* 直播主要问题是延迟大，无论怎么切片，延迟一般都会在2s以上

### rtmp
* 一般用于直播，延迟能够做到1s以内，但弱网环境下不行
* 直播推流一般用RTMP

* 基于tcp，弱网环境下丢包严重
* adobe私有协议，更新不足，浏览器上需要用flash插件，也不支持了

### webrtc
* W3C标准，主流浏览器和H5支持，相当于各平台都支持（包括不同操作系统和pc/移动端）
* 基于SRTP 和 UDP，弱网情况优化空间大
* 本质上是P2P，通信双方延迟低

* ICE、STUN、TURN等传统CDN没有类似服务出现