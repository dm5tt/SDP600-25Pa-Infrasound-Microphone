import numpy as np
import matplotlib.pyplot as plt
import scipy.signal as signal


data = np.genfromtxt('data.txt', dtype=np.int16)

f, t, Sxx = signal.spectrogram(data, 1 / 0.00459)

fig, ax = plt.subplots()
im = ax.imshow(Sxx, aspect='auto', origin='lower', extent=[t[0], t[-1], f[0], f[-1]], cmap='jet',  vmin=0, vmax=15)
plt.colorbar(im)
plt.xlabel('Time [s]')
plt.ylabel('Frequency [Hz]')

plt.show()
