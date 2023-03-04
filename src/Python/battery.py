from datetime import timedelta
import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.dates as mdates
import os

# Define duration going back
duration = "3d"

# Raw payload directory
directory = os.getcwd() + "/data/LoRa/battery.csv"

# Bash command to download full payload for specified duration
bashCommand = "curl -o \"" + directory + \
    "\" \"https://prototype.asap-forecast.com/logs/ormstrup-au?filter=ttn-payload&back=" + duration + "\""

# Execute bash command
os.system(bashCommand)

# Import data
data = pd.read_csv('./data/LoRa/battery.csv')

# Initialize variables
level = []
voltage = []
time = []

# Extract payloads containing battery readings
for column in data.columns:
    # Get LoRa message timestamp
    if ("createdAt" in column):
        loraMsg = column.split('"')[1]
        timeMsg = loraMsg.split('.')
        timeObject = pd.to_datetime(timeMsg[0], format='%Y-%m-%dT%H:%M:%S')
        timeObject += timedelta(hours=2)
        time.append(timeObject)

    # Payload data
    if ("payload_hex:" in column):
        loraMsg = column.split('"')[1]
        payload = loraMsg.split(' ')
        try:
            # Battery reading message identifier
            if (payload[1] == "3"):
                level.append(int('0x'+payload[2], 0))
                voltage.append(int('0x'+payload[3], 0) / 10)
            # Message was not battery reading, remove last payload time
            else:
                time.pop()
        # Empty data package, remove last payload time
        except:
            time.pop()


# Plotting battery readings
fig, ax = plt.subplots()
ax.plot(time, level)
ax2 = ax.twinx()
ax2.plot(time, voltage, 'r')

# Plot data (x-axis) format
myFmt = mdates.DateFormatter('%d/%m - %H:%M')
ax.xaxis.set_major_formatter(myFmt)
plt.gcf().autofmt_xdate()

# Axis labels
plt.xlabel('Time')
ax.set_ylabel('Charge Level [%]')
ax2.set_ylabel('Battery Voltage [V]')
plt.title('Battery Level (Estimate)')

# Axis limits
ax.set_ylim(0, 101)         # 0 - 100%
ax2.set_ylim(10, 15)        # 10 - 15V


# ax.set_facecolor('xkcd:grey')
# fig.patch.set_facecolor('xkcd:grey')
# function to show the plot
plt.show()
