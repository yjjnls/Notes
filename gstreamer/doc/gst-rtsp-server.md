## clone
```
git clone git://anongit.freedesktop.org/gstreamer/gst-rtsp-server
```
## configure and build
### additional include path
```
D:\tmp\wms\build\build.debug\local\x86_64\include
D:\tmp\wms\build\build.debug\local\x86_64\include\gstreamer-1.0
D:\tmp\wms\build\build.debug\local\x86_64\include\glib-2.0
D:\tmp\wms\build\build.debug\local\x86_64\lib\glib-2.0\include
D:\tmp\wms\build\build.debug\local\x86_64\lib\gstreamer-1.0\include
```

### additional lib path
```
D:/tmp/wms/build/build.debug/local/x86_64/lib
D:/tmp/wms/build/build.debug/local/x86_64/lib/$(Configuration)
```

### link lib
```
gstrtspserver-1.0.lib
gstreamer-1.0.lib
gobject-2.0.lib
glib-2.0.lib
```

### environment
```
GST_PLUGIN_PATH=D:\tmp\wms\build\build.debug\local\x86_64\bin;D:\tmp\wms\build\build.debug\local\x86_64\lib\gstreamer-1.0
PATH=%PATH%;D:\tmp\wms\build\build.debug\local\x86_64\bin
```

## Run
### command-line
```
./test-launch "( videotestsrc ! x264enc ! rtph264pay name=pay0 pt=96 )"
```

videotestsrc ! video/x-raw,width=320,height=240,framerate=10/1 ! openh264enc ! rtph264pay name=pay0 pt=96 ! audiotestsrc ! audio/x-raw,rate=8000 ! alawenc ! rtppcmapay name=pay1 pt=97

videotestsrc ! video/x-raw,width=320,height=240,framerate=10/1 ! openh264enc ! rtph264pay name=pay0 pt=96 ! audiotestsrc ! audio/x-raw,rate=48000 ! opusenc ! rtpopuspay name=pay1 pt=97

- 这里的引号一定要有
### vlc
```
rtsp://127.0.0.1:8554/test
```