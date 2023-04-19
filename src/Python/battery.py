from datetime import timedelta
import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.dates as mdates
import numpy as np
import os

# Define duration going back
duration = "5d"

# Enable Legacy payloads
legacy = False

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


# Extract integer from payload
def parsePayloadInt(payload, startIndex, length=4):
    # Add hex identifier
    byteString = ['0x' + s for s in payload[startIndex:startIndex+length]]

    # Convert to byteframe
    frame = b""
    for s in byteString:
        frame += bytes(int(s, 16).to_bytes(1, "big"))

    # Convert to integer values
    intValue = int.from_bytes(frame, "big", signed=True)

    return intValue


# Extract payloads containing battery readings
for column in data.columns:
    # Get LoRa message timestamp
    if ("createdAt" in column):
        loraMsg = column.split('"')[1]
        timeMsg = loraMsg.split('.')
        timeObject = pd.to_datetime(timeMsg[0], format='%Y-%m-%dT%H:%M:%S')
        timeObject += timedelta(hours=1)
        time.append(timeObject)

    # Payload data
    if ("payload_hex:" in column):
        loraMsg = column.split('"')[1]
        payload = loraMsg.split(' ')
        try:
            # Battery reading message identifier
            if (payload[1] == "3"):
                level.append(int('0x'+payload[2], 0))
                if (legacy):
                    voltage.append(int('0x'+payload[3], 0) / 10)
                else:
                    voltage.append(parsePayloadInt(payload, 3, 2) / 100)

            # Message was not battery reading, remove last payload time
            else:
                time.pop()
        # Empty data package, remove last payload time
        except:
            time.pop()


# Set style and Create figure
plt.style.use('bmh')
fig, ax = plt.subplots(figsize=(10, 6))

# Plot Values
handle1 = ax.plot(time, level, label='Battery Level [%]')
ax2 = ax.twinx()
handle2 = ax2.plot(
    time, voltage, color=plt.rcParams['axes.prop_cycle'].by_key()['color'][1], label='Battery Voltage [V]')


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
ax.set_ylim(0, 100)         # 0 - 100%
ax2.set_ylim(np.floor(min(voltage)-1),
             np.ceil(max(voltage)+1))        # 10 - 15V

# Set y-axis tick labels on ax and ax2
ax.set_yticks(np.linspace(0, 100, 9))
ax2.set_yticks(np.linspace(ax2.get_ylim()[0], ax2.get_ylim()[1], 9))

# Add grid to both axes
ax.grid(color='lightgray', linewidth=0.5)

# Plot Legend
handles = handle1 + handle2
labels = [l.get_label() for l in handles]
ax.legend(handles, labels, loc='upper center',
          bbox_to_anchor=(0.5, 1.00), shadow=True, ncol=2)

# function to show the plot
plt.savefig('./data/LoRa/battery.pdf')
plt.show()
