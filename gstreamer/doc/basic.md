
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
