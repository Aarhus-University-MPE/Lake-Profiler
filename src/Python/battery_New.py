import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.dates as mdates
import requests
from datetime import timedelta

# Define duration going back
duration = "6h"

# Enable Legacy payloads
legacy = False

# Define URL to download data
url = "https://prototype.asap-forecast.com/logs/ormstrup-au"

# Define parameters for the request
params = {
    "filter": "ttn-payload",
    "back": duration
}

# Download data and load into DataFrame
response = requests.get(url, params=params)
data = pd.DataFrame(response.json())

# Initialize variables
level = []
voltage = []
time = []

# Extract payloads containing battery readings
for item in data:
    payload = item.get("payload_hex", "").split()
    if len(payload) > 2 and payload[1] == "3":
        level.append(int(payload[2], 0))
        voltage.append(
            int(payload[3], 0) / 10 if legacy else int(payload[3]+payload[4], 16) / 100)
        time.append(pd.to_datetime(
            item.get("createdAt", "")[:-5]) + timedelta(hours=1))

# Plotting battery readings
fig, ax = plt.subplots()
ax.plot(time, level, label='Battery Level [%]')
ax2 = ax.twinx()
ax2.plot(time, voltage, 'r', label='Battery Voltage [V]')
ax.xaxis.set_major_formatter(mdates.DateFormatter('%d/%m - %H:%M'))
plt.gcf().autofmt_xdate()
ax.set_ylabel('Charge Level [%]')
ax2.set_ylabel('Battery Voltage [V]')
ax.set_ylim(0, 101)
ax2.set_ylim(min(voltage)-1, max(voltage)+1)
handles, labels = ax.get_legend_handles_labels()
handles2, labels2 = ax2.get_legend_handles_labels()
ax.legend(handles+handles2, labels+labels2, loc=0)
plt.title('Battery Level (Estimate)')
plt.show()
plt.savefig('./data/LoRa/battery')
