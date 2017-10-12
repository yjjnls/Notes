
<font face="微软雅黑">

- [gst-launch](#gst-launch)
    - [1.Basics](#1basics)
        - [设置属性](#%E8%AE%BE%E7%BD%AE%E5%B1%9E%E6%80%A7)
        - [带名称的element](#%E5%B8%A6%E5%90%8D%E7%A7%B0%E7%9A%84element)
        - [设置pad](#%E8%AE%BE%E7%BD%AEpad)
    - [2.Examples](#2examples)
        - [直接浏览视频流](#%E7%9B%B4%E6%8E%A5%E6%B5%8F%E8%A7%88%E8%A7%86%E9%A2%91%E6%B5%81)
            - [浏览测试视频](#%E6%B5%8F%E8%A7%88%E6%B5%8B%E8%AF%95%E8%A7%86%E9%A2%91)
            - [浏览ipc码流](#%E6%B5%8F%E8%A7%88ipc%E7%A0%81%E6%B5%81)
                - [playbin](#playbin)
                - [uridecodebin](#uridecodebin)
                - [rtspsrc](#rtspsrc)
        - [录制视频流](#%E5%BD%95%E5%88%B6%E8%A7%86%E9%A2%91%E6%B5%81)
            - [videotestsrc](#videotestsrc)

# gst-launch
## 1.Basics
gst-launch将多个**element**连成一个pipeline，用于快速测试、验证功能等。elements之间用感叹号进行连接。
```
gst-launch-1.0.exe videotestsrc ! autovideosink
```

### 设置属性
element可以是有属性的，用“属性=值”的方式设置属性，多个属性用空格分开。可以用gst-inspect工具来查看element的属性。
```
gst-launch-1.0.exe videotestsrc pattern=11 ! autovideosink
```

### 带名称的element
如果在后续要使用之前创建的element，则该element必须要设置名称属性，带名字的element在使用名字时需要在后面加一个点。
```
gst-launch-0.10 videotestsrc ! ffmpegcolorspace ! tee name=t ! queue ! autovideosink t. ! queue ! autovideosink  
```

### 设置pad
一个element可能会有多个pad，如果要指定pad进行连接，那么可以在命名element后使用.+pad名字来实现。
```
gst-launch-0.10.exe souphttpsrc location=http://docs.gstreamer.com/media/sintel_trailer-480p.webm ! matroskademux name=d d.video_00 ! matroskamux ! filesink location=sintel_video.mkv  
```
这个命令使用souphttpsrc在internet上锁定了一个媒体文件，这个文件是webm格式的。我们可以用matroskademux来打开这个文件，因为媒体包含音频和视频，所以我们创建了两个输出Pad，名字分别是video_00和audio_00。我们把video_00和matroskamux element连接起来，把视频流重新打包，最后连接到filesink，这样我们就把流存到了一个名叫sintel_video.mkv的文件。

----------------
## 2.Examples
### 直接浏览视频流
#### 浏览测试视频
```
gst-launch-1.0 videotestsrc ! autovideosink
```
#### 浏览ipc码流
##### playbin
```
gst-launch-1.0 playbin uri=rtsp://172.16.66.66:554/id=1
```
##### uridecodebin
```
gst-launch-1.0 uridecodebin uri=rtsp://172.16.66.66:554/id=1 ! autovideosink
```
##### rtspsrc
```
gst-launch-1.0 rtspsrc location=rtsp://172.16.66.66:554/id=1 ! rtph264depay ! avdec_h264 ! autovideosink
```
* __rtph264depay__ 用于将rtp包解包
* __avdec_h264__ 将h264码流解码

### 录制视频流
#### videotestsrc
```
gst-launch-1.0 -e videotestsrc ! video/x-raw, framerate=25/1, width=640, height=360 ! x264enc ! filesink location=test.ts
```
* __video/x-raw__ 设置流的类型，也可以不设置采用默认类型
* __x264enc__ 将原始码流编码，再保存成文件