import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from datetime import datetime
from scipy.signal import spectrogram

data = pd.read_csv('example.txt', names=['timestamp', 'value'])

time_diffs = np.diff(data['timestamp'] / 1e6) 

average_sample_rate = 1 / np.mean(time_diffs)

print(f"Average Sampling Time: {average_sample_rate:.2f} Hz")

fs = int(average_sample_rate) 

time_vector = np.linspace(0, len(data) / average_sample_rate, len(data))

frequencies, times, Sxx = spectrogram(data['value'], fs)

plt.pcolormesh(times, frequencies, Sxx, shading='auto', vmax=0.0001)
plt.ylabel('Frequency [Hz]')
plt.xlabel('Time [s]')
plt.title('Spectrogram')
plt.colorbar(label='Pa')

plt.subplots_adjust(bottom=0.2) 

plt.show()
