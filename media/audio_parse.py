# -*- coding: UTF-8 -*-
# gst-launch-1.0.exe audiotestsrc freq=5000 num-buffers=500 ! wavenc ! filesink location=test.wav

import wave
import pyaudio
import numpy
import pylab
import math
#打开WAV文档，文件路径根据需要做修改
wf = wave.open("test.wav", "rb")
#创建PyAudio对象
p = pyaudio.PyAudio()
stream = p.open(
    format=p.get_format_from_width(wf.getsampwidth()),
    channels=wf.getnchannels(),
    rate=wf.getframerate(),
    output=True)
nframes = wf.getnframes()
framerate = wf.getframerate()
nchannels = wf.getnchannels()
sample_width = wf.getsampwidth()

#读取完整的帧数据到str_data中，这是一个string类型的数据
str_data = wf.readframes(nframes)
wf.close()

#将波形数据转换为数组
# A new 1-D array initialized from raw binary or text data in a string.
wave_data = numpy.fromstring(str_data, dtype=numpy.short)
# 多路音频重组
wave_data = numpy.reshape(wave_data, [nframes, nchannels]).T

# 采样点数，修改采样点数和起始位置进行不同位置和长度的音频波形分析
N = 44100
start = 0  #开始采样位置
df = framerate / (N - 1)  # 分辨率
freq = [df * n for n in range(0, N)]  #N个元素
wave_data2 = wave_data[0][start:start + N]  #取第一个通道
c = numpy.fft.fft(wave_data2) * 2 / N

#常规显示采样频率一半的频谱
d = int(len(c) / 2)

freqency = freq[:d - 1]
amplitude = abs(c[:d - 1])
total_energy = 0
target_energy = 0
for index in range(0, len(amplitude)):
    total_energy += amplitude[index]**2
    if index > 4000 and index < 6000:
        target_energy += amplitude[index]**2

print target_energy / total_energy

pylab.plot(freqency, amplitude, 'b')
pylab.show()