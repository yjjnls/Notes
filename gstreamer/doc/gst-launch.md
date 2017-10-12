
<font face="微软雅黑">

- [1.Basics](#1basics)
    - [1.1 设置属性](#11-%E8%AE%BE%E7%BD%AE%E5%B1%9E%E6%80%A7)
    - [1.2 带名称的element](#12-%E5%B8%A6%E5%90%8D%E7%A7%B0%E7%9A%84element)
    - [1.3 设置pad](#13-%E8%AE%BE%E7%BD%AEpad)
- [2. Examples](#2-examples)
    - [2.1 直接浏览视频流](#21-%E7%9B%B4%E6%8E%A5%E6%B5%8F%E8%A7%88%E8%A7%86%E9%A2%91%E6%B5%81)
        - [2.1.1 浏览测试视频](#211-%E6%B5%8F%E8%A7%88%E6%B5%8B%E8%AF%95%E8%A7%86%E9%A2%91)
        - [2.1.2 浏览ipc码流](#212-%E6%B5%8F%E8%A7%88ipc%E7%A0%81%E6%B5%81)
            - [2.1.2.1 playbin](#2121-playbin)
            - [2.1.2.2 uridecodebin](#2122-uridecodebin)
            - [2.1.2.3 rtspsrc](#2123-rtspsrc)
    - [2.2 录制视频流](#22-%E5%BD%95%E5%88%B6%E8%A7%86%E9%A2%91%E6%B5%81)
        - [2.2.1 videotestsrc](#221-videotestsrc)
        - [2.2.2 rtspsrc](#222-rtspsrc)
    - [2.3 Rtsp推流](#23-rtsp%E6%8E%A8%E6%B5%81)
        - [2.3.1 filesrc](#231-filesrc)
        - [2.3.2 rtspsrc](#232-rtspsrc)
        - [2.3.3 receive](#233-receive)


# 1.Basics
gst-launch将多个**element**连成一个pipeline，用于快速测试、验证功能等。elements之间用感叹号进行连接。
```
gst-launch-1.0.exe videotestsrc ! autovideosink
```

## 1.1 设置属性
element可以是有属性的，用“属性=值”的方式设置属性，多个属性用空格分开。可以用gst-inspect工具来查看element的属性。
```
gst-launch-1.0.exe videotestsrc pattern=11 ! autovideosink
```

## 1.2 带名称的element
如果在后续要使用之前创建的element，则该element必须要设置名称属性，带名字的element在使用名字时需要在后面加一个点。
```
gst-launch-0.10 videotestsrc ! ffmpegcolorspace ! tee name=t ! queue ! autovideosink t. ! queue ! autovideosink  
```

## 1.3 设置pad
一个element可能会有多个pad，如果要指定pad进行连接，那么可以在命名element后使用.+pad名字来实现。
```
gst-launch-0.10.exe souphttpsrc location=http://docs.gstreamer.com/media/sintel_trailer-480p.webm ! matroskademux name=d d.video_00 ! matroskamux ! filesink location=sintel_video.mkv  
```
这个命令使用souphttpsrc在internet上锁定了一个媒体文件，这个文件是webm格式的。我们可以用matroskademux来打开这个文件，因为媒体包含音频和视频，所以我们创建了两个输出Pad，名字分别是video_00和audio_00。我们把video_00和matroskamux element连接起来，把视频流重新打包，最后连接到filesink，这样我们就把流存到了一个名叫sintel_video.mkv的文件。

----------------
# 2. Examples
## 2.1 直接浏览视频流
### 2.1.1 浏览测试视频
```
gst-launch-1.0 videotestsrc ! autovideosink
```
### 2.1.2 浏览ipc码流
#### 2.1.2.1 playbin
```
gst-launch-1.0 playbin uri=rtsp://172.16.66.66:554/id=1
```
#### 2.1.2.2 uridecodebin
```
gst-launch-1.0 uridecodebin uri=rtsp://172.16.66.66:554/id=1 ! autovideosink
```
#### 2.1.2.3 rtspsrc
```
gst-launch-1.0 rtspsrc location=rtsp://172.16.66.66:554/id=1 ! rtph264depay ! avdec_h264 ! autovideosink
```
* __rtph264depay__ 用于将rtp包解包
* __avdec_h264__ 将h264码流解码

## 2.2 录制视频流
### 2.2.1 videotestsrc
```
gst-launch-1.0 -e videotestsrc ! video/x-raw, framerate=25/1, width=640, height=360 ! x264enc ! filesink location=test.ts
```
* __video/x-raw__ 设置流的类型，也可以不设置采用默认类型
* __x264enc__ 将原始码流编码，再保存成文件

### 2.2.2 rtspsrc
```
win
gst-launch-1.0 rtspsrc location=rtsp://172.16.66.66:554/id=1 ! rtph264depay ! h264parse ! avdec_h264 ! x264enc  ! filesink location=test2.ts

linux
gst-launch-1.0 rtspsrc location=rtsp://172.16.66.66:554/id=1 ! rtph264depay ! h264parse ! avdec_h264 ! x264enc  ! filesink location=/mnt/hgfs/wintmp/wms/test2.ts

```
- 没有 ! avdec_h264 ! x264enc ,录制的文件无法播放
- h264parse 可以不要

## 2.3 Rtsp推流
### 2.3.1 filesrc 
### 2.3.2 rtspsrc 
### 2.3.3 receive
```
gst-launch-1.0 udpsrc port=5000 ! application/x-rtp, clock-rate=90000,payload=96 ! rtph264depay !  avdec_h264 ! autovideosink
```