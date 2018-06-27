<font face="微软雅黑">

- [1. gst-launch](#1-gst-launch)
    - [1.1 Basics](#11-basics)
        - [1.1.1 设置属性](#111-%E8%AE%BE%E7%BD%AE%E5%B1%9E%E6%80%A7)
        - [1.1.2 带名称的element](#112-%E5%B8%A6%E5%90%8D%E7%A7%B0%E7%9A%84element)
        - [1.1.3 设置pad](#113-%E8%AE%BE%E7%BD%AEpad)
        - [1.1.4 caps过滤](#114-caps%E8%BF%87%E6%BB%A4)
        - [1.1.5 查看pipeline中的caps](#115-%E6%9F%A5%E7%9C%8Bpipeline%E4%B8%AD%E7%9A%84caps)
    - [1.2. Examples](#12-examples)
        - [1.2.1 直接浏览视频流](#121-%E7%9B%B4%E6%8E%A5%E6%B5%8F%E8%A7%88%E8%A7%86%E9%A2%91%E6%B5%81)
            - [1.2.1.1 test video](#1211-test-video)
            - [1.2.1.2 ipc码流](#1212-ipc%E7%A0%81%E6%B5%81)
                - [1.2.1.2.1 playbin](#12121-playbin)
                - [1.2.1.2.2 uridecodebin](#12122-uridecodebin)
                - [1.2.1.2.3 rtspsrc](#12123-rtspsrc)
        - [1.2.2 录制视频流](#122-%E5%BD%95%E5%88%B6%E8%A7%86%E9%A2%91%E6%B5%81)
            - [1.2.2.1 test video](#1221-test-video)
            - [1.2.2.2 ipc](#1222-ipc)
        - [1.2.3 Rtsp推流](#123-rtsp%E6%8E%A8%E6%B5%81)
            - [1.2.3.1 file](#1231-file)
            - [1.2.3.2 ipc](#1232-ipc)
            - [1.2.3.3 receive](#1233-receive)
- [Extensions](#extensions)
- [test server](#test-server)

# 1. gst-launch

## 1.1 Basics

gst-launch将多个**element**连成一个pipeline，用于快速测试、验证功能等。elements之间用感叹号进行连接。

    gst-launch-1.0.exe videotestsrc ! autovideosink

### 1.1.1 设置属性

element可以是有属性的，用“属性=值”的方式设置属性，多个属性用空格分开。可以用gst-inspect工具来查看element的属性。

    gst-launch-1.0.exe videotestsrc pattern=11 ! autovideosink

### 1.1.2 带名称的element

如果在后续要使用之前创建的element，则该element必须要设置名称属性，带名字的element在使用名字时需要在后面加一个点。

    gst-launch-0.10 videotestsrc ! ffmpegcolorspace ! tee name=t ! queue ! autovideosink t. ! queue ! autovideosink  

### 1.1.3 设置pad

一个element可能会有多个pad，如果要指定pad进行连接，那么可以在命名element后使用.+pad名字来实现。

    gst-launch-0.10.exe souphttpsrc location=http://docs.gstreamer.com/media/sintel_trailer-480p.webm ! matroskademux name=d d.video_00 ! matroskamux ! filesink location=sintel_video.mkv  

这个命令使用souphttpsrc在internet上锁定了一个媒体文件，这个文件是webm格式的。我们可以用matroskademux来打开这个文件，因为媒体包含音频和视频，所以我们创建了两个输出Pad，名字分别是video_00和audio_00。我们把video_00和matroskamux element连接起来，把视频流重新打包，最后连接到filesink，这样我们就把流存到了一个名叫sintel_video.mkv的文件。

### 1.1.4 caps过滤

除了上述指定pad的方法外，还可以设置caps过滤，设置当前element的输出类型。

    gst-launch-0.10 souphttpsrc location=http://docs.gstreamer.com/media/sintel_trailer-480p.webm ! matroskademux ! video/x-vp8 ! matroskamux ! filesink location=sintel_video.mkv  

在matroskademux后加一个**video/x-vp8的Caps过滤**，这样就表明在matroskademux中我们仅仅需要能生成这种类型视频的输出Pad。

### 1.1.5 查看pipeline中的caps

**在gst-launch后加 -v 选项，就可以查看pipeline中每个element具体选择了哪些pad**

    gst-launch-1.0 -v videotestsrc! autovideosink

> > /GstPipeline:pipeline0/GstVideoTestSrc:videotestsrc0.GstPad:src: caps = video/x-raw, format=(string)I420, width=(int)320, height=(int)240, framerate=(fraction)30/1, multiview-mode=(string)mono, pixel-aspect-ratio=(fraction)1/1, interlace-mode=(string)progressive
>
> > /GstPipeline:pipeline0/GstAutoVideoSink:autovideosink0.GstGhostPad:sink.GstProxyPad:proxypad0: caps = video/x-raw, format=(string)I420, width=(int)320, height=(int)240, framerate=(fraction)30/1, multiview-mode=(string)mono, pixel-aspect-ratio=(fraction)1/1, interlace-mode=(string)progressive
>
> > /GstPipeline:pipeline0/GstAutoVideoSink:autovideosink0/GstD3DVideoSink:autovideosink0-actual-sink-d3dvideo.GstPad:sink: caps = video/x-raw, format=(string)I420, width=(int)320, height=(int)240, framerate=(fraction)30/1, multiview-mode=(string)mono, pixel-aspect-ratio=(fraction)1/1, interlace-mode=(string)progressive
>
> > /GstPipeline:pipeline0/GstAutoVideoSink:autovideosink0.GstGhostPad:sink: caps = video/x-raw, format=(string)I420, width=(int)320, height=(int)240, framerate=(fraction)30/1, multiview-mode=(string)mono, pixel-aspect-ratio=(fraction)1/1, interlace-mode=(string)progressive

* * *

## 1.2. Examples

### 1.2.1 直接浏览视频流

#### 1.2.1.1 test video

    gst-launch-1.0 videotestsrc ! autovideosink

#### 1.2.1.2 ipc码流

##### 1.2.1.2.1 playbin

    gst-launch-1.0 playbin uri=rtsp://172.16.66.66:554/id=1

##### 1.2.1.2.2 uridecodebin

    gst-launch-1.0 uridecodebin uri=rtsp://172.16.66.66:554/id=1 ! autovideosink

##### 1.2.1.2.3 rtspsrc

    gst-launch-1.0 rtspsrc location=rtsp://172.16.66.66:554/id=1 ! rtph264depay ! avdec_h264 ! autovideosink

    gst-launch-1.0 rtspsrc location=rtsp://172.16.66.66:554/id=1 ! decodebin3 ! autovideosink

- Rtsp浏览MJEPG码流（码流源就是一张张的jpeg）  
 
```
gst-launch-1.0.exe rtspsrc location="rtsp://172.16.66.65:554/realtime?id=0;aid=0;agent=onvif" ! rtpjpegdepay ! jpegdec ! autovideosink
```

-   **rtph264depay** 用于将rtp包解包
-   **avdec_h264** 将h264码流解码

### 1.2.2 录制视频流

把视频流编码成H264格式，并转为MPEG-TS传输流。

#### 1.2.2.1 test video

    win
    gst-launch-1.0 -e videotestsrc ! video/x-raw, framerate=25/1, width=640, height=360 ! x264enc ! filesink location=test.ts

    linux
    gst-launch-1.0 -e videotestsrc ! video/x-raw, framerate=25/1, width=640, height=360 ! x264enc ! filesink location=/mnt/hgfs/wintmp/wms/test.ts

-   **video/x-raw** 设置流的类型，也可以不设置采用默认选择
-   **x264enc** 将原始码流编码，再保存成文件
-   **-e** 选项可以在我们关闭pipeline之前强制关闭给视频源发出EOS信号，适用于用CTRL+C或者kill命令来关闭gst-launch

也可以加上混合器**mpegtsmux**  

    gst-launch-1.0 -e videotestsrc ! video/x-raw, framerate=25/1, width=640, height=360 ! x264enc ! mpegtsmux ! filesink location=test.ts

#### 1.2.2.2 ipc

    win
    gst-launch-1.0 -e rtspsrc location=rtsp://172.16.66.66:554/id=1 ! rtph264depay ! h264parse ! avdec_h264 ! x264enc  ! filesink location=test2.ts

    linux
    gst-launch-1.0 -e rtspsrc location=rtsp://172.16.66.66:554/id=1 ! rtph264depay ! h264parse ! avdec_h264 ! x264enc  ! filesink location=/mnt/hgfs/wintmp/wms/test2.ts

-   没有 ! avdec_h264 ! x264enc ,录制的文件无法播放
-   h264parse 可以不用

### 1.2.3 Rtsp推流

#### 1.2.3.1 file

    win
    gst-launch-1.0 -v filesrc location="D:\\tmp\\wms\\build\\build.debug\\local\\x86_64\\test.ts" ! h264parse ! rtph264pay config-interval=1 pt=96 ! udpsink host=127.0.0.1 port=5000

    linux
    gst-launch-1.0 -v filesrc location=/mnt/hgfs/wintmp/wms/test.ts ! h264parse ! rtph264pay config-interval=1 pt=96 ! udpsink host=172.16.64.68 port=5000

-   **host**为目标ip
-   如果录制文件时用了混合器，则在推流时要用相应的分离，例如**tsdemux**


    gst-launch-1.0 -v filesrc location="D:\\tmp\\wms\\build\\build.debug\\local\\x86_64\\test.ts" ! tsdemux ! h264parse ! rtph264pay config-interval=1 pt=96 ! udpsink host=127.0.0.1 port=5000

#### 1.2.3.2 ipc

    win
    gst-launch-1.0 rtspsrc location=rtsp://172.16.66.66:554/id=1 ! rtph264depay ! h264parse ! rtph264pay config-interval=1 pt=96 ! udpsink host=127.0.0.1 port=5000

    linux
    gst-launch-1.0 rtspsrc location=rtsp://172.16.66.66:554/id=1 ! rtph264depay ! h264parse ! rtph264pay config-interval=1 pt=96 ! udpsink host=172.16.64.58 port=5000

#### 1.2.3.3 receive

    gst-launch-1.0 udpsrc port=5000 ! application/x-rtp, clock-rate=90000,payload=96 ! rtph264depay !  avdec_h264 ! autovideosink

# Extensions

[GStreamer基础教程10——GStreamer工具](http://blog.csdn.net/sakulafly/article/details/21455637)

[GStreamer基础教程14——常用的element](http://blog.csdn.net/sakulafly/article/details/21748777)

[Gstreamer cheat sheet](http://wiki.oz9aec.net/index.php/Gstreamer_cheat_sheet)


# test server

rtsp-server-test "( videotestsrc pattern=white ! timeoverlay valignment=3 halignment=4 time-mode=2 xpos=0 ypos=0 color=4278190080 font-desc=\"Sans 48\" draw-shadow=false draw-outline=falseoutline-color=4278190080  ! video/x-raw, format=(string)I420, width=(int)320, height=(int)240  ! x264enc bitrate=512 key-int-max=5 ! rtph264pay pt=96 name=pay0  audiotestsrc freq=440 wave=sine ! audio/x-raw, rate=(int)8000, channels=(int)1 ! alawenc ! rtppcmapay pt=97 name=pay1 )"

gst-launch-1.0.exe rtspsrc location=rtsp://127.0.0.1:8553/test ! rtph264depay ! avdec_h264 ! jpegenc ! multifilesink location="test/%05d.bmp"