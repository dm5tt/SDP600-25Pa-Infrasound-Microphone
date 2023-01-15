import pandas as pd
import matplotlib.pyplot as plt
from datetime import datetime

data = pd.read_csv('example.txt', names=['timestamp', 'value'])

data['timestamp'] = pd.to_datetime(data['timestamp'], unit='us')

data['time_minutes'] = (data['timestamp'] - data['timestamp'].dt.normalize()).dt.total_seconds() / 60

# Plot erstellen
fig, ax = plt.subplots()
ax.plot(data['time_minutes'], data['value'], marker='', linestyle='-', color='b')  # Keine Marker, feste Linie

ax.set_xlabel('time (hh:mm)')
ax.set_ylabel('Pressure difference in Pa')
ax.set_title('Pressure difference  Plot')

ax.xaxis.set_major_formatter(plt.FuncFormatter(lambda x, _: '{:02}:{:02}'.format(int(x // 60), int(x % 60))))

plt.subplots_adjust(bottom=0.2)

plt.show()
