## 原理设计
这里利用RTCMultiConnection的demo `Video-Conferencing` 进行改造来实现多点连接。原始demo界面如下，分为open room和join room两个操作，每个用户都能看到所有参与者的画面。

![](img/multipoints1.png)

我们这里将其进行简化，每个房间限制为只能有两个参与者进行一对一的链接，这样一个room就退化成了一个connection，一端为网页用户，一端为gstreamer webrtc。将多个gstreamer webrtc并联在一起就形成了多点链接。

![](img/multipoints2.png)


## 时序图


![](img/multipoints3.png)

## 接口设计

