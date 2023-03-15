from datetime import timedelta
import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.dates as mdates
import os

# Define duration going back
duration = "2h"

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
                    print(parsePayloadInt(payload, 3, 2) / 100)
                    voltage.append(parsePayloadInt(payload, 3, 2) / 100)

            # Message was not battery reading, remove last payload time
            else:
                time.pop()
        # Empty data package, remove last payload time
        except:
            time.pop()


# Plotting battery readings
fig, ax = plt.subplots()
handle1 = ax.plot(time, level, label='Battery Level [%]')
ax2 = ax.twinx()
handle2 = ax2.plot(time, voltage, 'r', label='Battery Voltage [V]')

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
ax2.set_ylim(min(voltage)-1, max(voltage)+1)        # 10 - 15V

handles = handle1 + handle2
labels = [l.get_label() for l in handles]
ax.legend(handles, labels, loc=0)

# ax.set_facecolor('xkcd:grey')
# fig.patch.set_facecolor('xkcd:grey')
# function to show the plot
# plt.legend(['Level', 'Voltage'])
plt.show()
plt.savefig('./data/LoRa/battery')
