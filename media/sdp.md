# Introduction

SDP属于一种会话描述格式，主要用于通信双方对于特定内容的协商，比如会话的名称与目的、接收信息的地址、端口、格式等等。SDP描述由许多文本行组成，文本行的格式为：

>  &lt; type > = &lt; value > [CRLF]

-   type: 该字节为单字节（如： v，o, m等）区分大小写，=号俩侧部允许有空格
-   value: 为结构化文本串

## 一些常用的字段

-   **v=0 sdp版本**
-   **o=(用户名)(会话标识)(版本)(网络类型)(地址类型)(地址)**  
    会话源，如果不存在用户登录名，该字段标志位“-”  
    会话标识为一随机数字串 版本为该会话公告的版本  
    网络类型为文本串， “IN”表示internet  
    地址类型为文本串，目前定义为“IP4”和“IP6”两种地址  
-   s=(会话名)  
    每个会话描述必须只有一个会话名
-   i=(会话信息)  
    此字段并非必须，建议包括进来用于描叙相应会话文字性说明，每个会话描叙最多只能有一个
-   t = （会话活动时间）
-   r = \* （0或多次重复次数）
-   **m = （媒体名称和传输地址）**
-   i = \* （媒体标题）
-   c = \* (网络类型)（地址类型）（连接地址）  
    表示媒体连接信息。
     一个会话声明中，会话级描述中必须有”c=”项或者在每个媒体级描述中有一个”c=”项。
-   b = \* （带宽信息）
-   k = \* （加密密钥）
-   **a = \* （0 个或多个会话属性行）**  
    **v o s t m是必须的**

### m描叙行：

格式： m=（媒体）（端口）（传送层协议）（格式列表）

媒体类型：5种。音频（audio），视频（video），应用，数据和控制

端口：媒体流发往传输层的端口

传送层协议：ip4上大多基于rtp/udp上传送（RTP/AVP）IETF RTP协议，在udp上传输  

格式列表： 对音/视频，就是音/视频应用文档中规定媒体净荷类型。

> m=video 0 RTP/AVP 96

### a描叙行：

格式：	
a=属性或a=属性:值  

> a=sendrecv  
> a=ptime:20  
>
> a=rtpmap:96 H264/90000

* * *

a=fmtp:&lt;格式>&lt;格式特定参数>//定义指定格式的附加参数

> a=fmtp:97 profile-level-id=42e01f;level-asymmetry-allowed=1

这里的97表示H264
该属性值指定一个SDP不了解的特殊格式，该格式是媒体的格式的一种。  
定义指定格式的附加参数  

a=rtpmap:&lt;净荷类型号>&lt;编码名>/&lt;时钟速率>[/<编码参数>]对音频，编码参数为音频信道数；对视频没有定义

> a=rtpmap:96 H264/90000

rtpmap属性对"m="描述的格式列表的负荷类型进行详细说明  
通用的rtpmap属性的格式为:
a=rtpmap:&lt; payload type> &lt; encoding name>/&lt; clock rate>[/< encoding parameters>]  
&lt; payload type >  
&lt; encoding name >：编码名，扩展的用X-开始  
&lt; clock rate >：采样频率  
&lt; encoding parameters >:对语音编码来说，该参数表示语音声道。  
对视频编码不提供该参数。

-----
sdp协商过程中的send payload和receive payload要根据offer中的mode来进行不同的设置

offer中的mode为recvonly
answer只设置send payload，且mode设为sendonly

offer中的mode为sendonly
answer只设置receive payload，且mode设为recvonly

offer中的mode为sendrecv
answer中两个payload都要设置，且mode也设置为sendrecv

answer的mode为snedonly或者sendrecv时，要另外设置ssrc和cname等
```cpp
mediaDescription.ssrc_group = it_offer_media_description->ssrc_group;
if (mediaDescription.ssrc_group.empty())
    mediaDescription.ssrc_group = "FID";
mediaDescription.ssrcs.push_back(send_ssrc);
mediaDescription.ssrcs.push_back(2231627014);
mediaDescription.cname = cname;
mediaDescription.mediastreamid = uuid_string_random();
mediaDescription.mediastreamtrackid = uuid_string_random();
```

offer的mode为recvonly或者sendrecv，才设置local source
```c
on_set_local_source(media_session, media_type, local_sources_);
```
