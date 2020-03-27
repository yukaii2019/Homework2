import matplotlib.pyplot as plt
import numpy as np
import serial
import time

Fs = 1024.0
Ts = 1.0/(Fs)
t = np.arange(0,1,Ts)
y = np.arange(0,1,Ts)

n = len(y)
k = np.arange(n)
T = n/Fs
frq = k/T
frq = frq[range(int (n/2))]

serdev = '/dev/ttyACM0'
s = serial.Serial(serdev)
for x in range(0,int(Fs)):
    line = s.readline()
    y[x] = float(line)

Y = np.fft.fft(y)/n*2
Y = Y[range(int(n/2))]

fig, ax = plt.subplots(2,1)
ax[0].plot(t,y)
ax[0].set_xlabel('Time')
ax[0].set_ylabel('Amplitude')
ax[1].plot(frq,abs(Y),'r')
ax[1].set_xlabel('Freq (Hz)')
ax[1].set_ylabel('|Y(freq)|')
plt.show()
s.close