<font face="微软雅黑">

- [1. gst-inspect](#1-gst-inspect)
    - [1.1 Basics](#11-basics)
    - [1.2 查看element的caps](#12-%E6%9F%A5%E7%9C%8Belement%E7%9A%84caps)
- [2. gst-discoverer](#2-gst-discoverer)

# 1. gst-inspect
## 1.1 Basics
* 不带参数，它会列出所有可用的element，也就是你所有可以使用的元素
* 带一个文件名，它会把这个文件作为GStreamer的一个插件，试着打开，然后列出内部所有的element
* 带一个GStreamer的element，会列出该element的所有信息
## 1.2 查看element的caps
```
gst-inspect-1.0 videotestsrc
```
```
Factory Details:
  Rank                     none (0)
  Long-name                Video test source
  Klass                    Source/Video
  Description              Creates a test video stream
  Author                   David A. Schleef <ds@schleef.org>

Plugin Details:
  Name                     videotestsrc
  Description              Creates a test video stream
  Filename                 D:\Working\Prj\Git\WMS\build\build\debug\local\x86_64\lib\gstreamer-1.0\libgstvideotestsrc.dll
  Version                  1.12.0
  License                  LGPL
  Source module            gst-plugins-base
  Source release date      2017-05-04
  Binary package           GStreamer Base Plug-ins source release
  Origin URL               Unknown package origin

GObject
 +----GInitiallyUnowned
       +----GstObject
             +----GstElement
                   +----GstBaseSrc
                         +----GstPushSrc
                               +----GstVideoTestSrc

Pad Templates:
  SRC template: 'src'
    Availability: Always
    Capabilities:
      video/x-raw
                 format: { (string)I420, (string)YV12, (string)YUY2, (string)UYVY, (string)AYUV, (string)RGBx, (string)BGRx, (string)xRGB, (string)xBGR, (string)RGBA, (string)BGRA, (string)ARGB, (string)ABGR, (string)RGB, (string)BGR, (string)Y41B, (string)Y42B, (string)YVYU, (string)Y444, (string)v210, (string)v216, (string)NV12, (string)NV21, (string)GRAY8, (string)GRAY16_BE, (string)GRAY16_LE, (string)v308, (string)RGB16, (string)BGR16, (string)RGB15, (string)BGR15, (string)UYVP, (string)A420, (string)RGB8P, (string)YUV9, (string)YVU9, (string)IYU1, (string)ARGB64, (string)AYUV64, (string)r210, (string)I420_10BE, (string)I420_10LE, (string)I422_10BE, (string)I422_10LE, (string)Y444_10BE, (string)Y444_10LE, (string)GBR, (string)GBR_10BE, (string)GBR_10LE, (string)NV16, (string)NV24, (string)NV12_64Z32, (string)A420_10BE, (string)A420_10LE, (string)A422_10BE, (string)A422_10LE, (string)A444_10BE, (string)A444_10LE, (string)NV61, (string)P010_10BE, (string)P010_10LE, (string)IYU2, (string)VYUY, (string)GBRA, (string)GBRA_10BE, (string)GBRA_10LE, (string)GBR_12BE, (string)GBR_12LE, (string)GBRA_12BE, (string)GBRA_12LE, (string)I420_12BE, (string)I420_12LE, (string)I422_12BE, (string)I422_12LE, (string)Y444_12BE, (string)Y444_12LE }
                  width: [ 1, 2147483647 ]
                 height: [ 1, 2147483647 ]
              framerate: [ 0/1, 2147483647/1 ]
         multiview-mode: { (string)mono, (string)left, (string)right }
      video/x-bayer
                 format: { (string)bggr, (string)rggb, (string)grbg, (string)gbrg }
                  width: [ 1, 2147483647 ]
                 height: [ 1, 2147483647 ]
              framerate: [ 0/1, 2147483647/1 ]
         multiview-mode: { (string)mono, (string)left, (string)right }


Element Flags:
  no flags set

Element Implementation:
  Has change_state() function: gst_base_src_change_state

Element has no clocking capabilities.
Element has no URI handling capabilities.

Pads:
  SRC: 'src'
    Pad Template: 'src'

Element Properties:
  name                : The name of the object
                        flags: readable, writable
                        String. Default: "videotestsrc0"
  parent              : The parent of the object
                        flags: readable, writable
                        Object of type "GstObject"
  blocksize           : Size in bytes to read per buffer (-1 = default)
                        flags: readable, writable
                        Unsigned Integer. Range: 0 - 4294967295 Default: 4096 
  num-buffers         : Number of buffers to output before sending EOS (-1 = unlimited)
                        flags: readable, writable
                        Integer. Range: -1 - 2147483647 Default: -1 
  typefind            : Run typefind before negotiating
                        flags: readable, writable
                        Boolean. Default: false
  do-timestamp        : Apply current stream time to buffers
                        flags: readable, writable
                        Boolean. Default: false
  pattern             : Type of test pattern to generate
                        flags: readable, writable
                        Enum "GstVideoTestSrcPattern" Default: 0, "smpte"
                           (0): smpte            - SMPTE 100% color bars
                           (1): snow             - Random (television snow)
                           (2): black            - 100% Black
                           (3): white            - 100% White
                           (4): red              - Red
                           (5): green            - Green
                           (6): blue             - Blue
                           (7): checkers-1       - Checkers 1px
                           (8): checkers-2       - Checkers 2px
                           (9): checkers-4       - Checkers 4px
                           (10): checkers-8       - Checkers 8px
                           (11): circular         - Circular
                           (12): blink            - Blink
                           (13): smpte75          - SMPTE 75% color bars
                           (14): zone-plate       - Zone plate
                           (15): gamut            - Gamut checkers
                           (16): chroma-zone-plate - Chroma zone plate
                           (17): solid-color      - Solid color
                           (18): ball             - Moving ball
                           (19): smpte100         - SMPTE 100% color bars
                           (20): bar              - Bar
                           (21): pinwheel         - Pinwheel
                           (22): spokes           - Spokes
                           (23): gradient         - Gradient
                           (24): colors           - Colors
  timestamp-offset    : An offset added to timestamps set on buffers (in ns)
                        flags: readable, writable
                        Integer64. Range: 0 - 2147483646999999999 Default: 0 
  is-live             : Whether to act as a live source
                        flags: readable, writable
                        Boolean. Default: false
  k0                  : Zoneplate zero order phase, for generating plain fields or phase offsets
                        flags: readable, writable
                        Integer. Range: -2147483648 - 2147483647 Default: 0 
  kx                  : Zoneplate 1st order x phase, for generating constant horizontal frequencies
                        flags: readable, writable
                        Integer. Range: -2147483648 - 2147483647 Default: 0 
  ky                  : Zoneplate 1st order y phase, for generating contant vertical frequencies
                        flags: readable, writable
                        Integer. Range: -2147483648 - 2147483647 Default: 0 
  kt                  : Zoneplate 1st order t phase, for generating phase rotation as a function of time
                        flags: readable, writable
                        Integer. Range: -2147483648 - 2147483647 Default: 0 
  kxt                 : Zoneplate x*t product phase, normalised to kxy/256 cycles per vertical pixel at width/2 from origin
                        flags: readable, writable
                        Integer. Range: -2147483648 - 2147483647 Default: 0 
  kyt                 : Zoneplate y*t product phase
                        flags: readable, writable
                        Integer. Range: -2147483648 - 2147483647 Default: 0 
  kxy                 : Zoneplate x*y product phase
                        flags: readable, writable
                        Integer. Range: -2147483648 - 2147483647 Default: 0 
  kx2                 : Zoneplate 2nd order x phase, normalised to kx2/256 cycles per horizontal pixel at width/2 from origin
                        flags: readable, writable
                        Integer. Range: -2147483648 - 2147483647 Default: 0 
  ky2                 : Zoneplate 2nd order y phase, normailsed to ky2/256 cycles per vertical pixel at height/2 from origin
                        flags: readable, writable
                        Integer. Range: -2147483648 - 2147483647 Default: 0 
  kt2                 : Zoneplate 2nd order t phase, t*t/256 cycles per picture
                        flags: readable, writable
                        Integer. Range: -2147483648 - 2147483647 Default: 0 
  xoffset             : Zoneplate 2nd order products x offset
                        flags: readable, writable
                        Integer. Range: -2147483648 - 2147483647 Default: 0 
  yoffset             : Zoneplate 2nd order products y offset
                        flags: readable, writable
                        Integer. Range: -2147483648 - 2147483647 Default: 0 
  foreground-color    : Foreground color to use (big-endian ARGB)
                        flags: readable, writable, controllable
                        Unsigned Integer. Range: 0 - 4294967295 Default: 4294967295 
  background-color    : Background color to use (big-endian ARGB)
                        flags: readable, writable, controllable
                        Unsigned Integer. Range: 0 - 4294967295 Default: 4278190080 
  horizontal-speed    : Scroll image number of pixels per frame (positive is scroll to the left)
                        flags: readable, writable
                        Integer. Range: -2147483648 - 2147483647 Default: 0 
  animation-mode      : For pattern=ball, which counter defines the position of the ball.
                        flags: readable, writable
                        Enum "GstVideoTestSrcAnimationMode" Default: 0, "frames"
                           (0): frames           - frame count
                           (1): wall-time        - wall clock time
                           (2): running-time     - running time
  motion              : For pattern=ball, what motion the ball does
                        flags: readable, writable
                        Enum "GstVideoTestSrcMotionType" Default: 0, "wavy"
                           (0): wavy             - Ball waves back and forth, up and down
                           (1): sweep            - 1 revolution per second
                           (2): hsweep           - 1/2 revolution per second, then reset to top
  flip                : For pattern=ball, invert colors every second.
                        flags: readable, writable
                        Boolean. Default: false

```

主要看**Pad Templates**属性，这里videtestsrc只包含src pad，表示只有输出。同时该pad有两个caps：**video/x-raw**和**video/x-bayer**。

如果是测试autovideosink，则会得到如下结果
```
Pad Templates:
  SINK template: 'sink'
    Availability: Always
    Capabilities:
      ANY
```
表示autovideosink可以接收任何类型的cap。

# 2. gst-discoverer
参数为文件，可以查看媒体文件的信息。比如查看录制的videotestsrc的录像文件，可以看到它的媒体属性。
```
Topology:
  video: H.264 (High Profile)

Properties:
  Duration: 0:02:46.347000000
  Seekable: yes
  Tags:
      video codec: H.264 (High Profile)
```
