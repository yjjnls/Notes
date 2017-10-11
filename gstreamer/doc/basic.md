
## gst-launch
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
