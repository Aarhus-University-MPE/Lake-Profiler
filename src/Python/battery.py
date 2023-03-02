from datetime import timedelta
import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.dates as mdates
import os

bashCommand = "curl -o \"D:/Projects/Lake-Profiler/data/LoRa/battery.csv\" \"https://prototype.asap-forecast.com/logs/ormstrup-au?filter=ttn-payload&back="
bashCommand += "2d\""

os.system(bashCommand)

data = pd.read_csv('./data/LoRa/battery.csv')

level = []
voltage = []
time = []

for column in data.columns:
    if ("createdAt" in column):
        loraMsg = column.split('"')[1]
        timeMsg = loraMsg.split('.')
        timeObject = pd.to_datetime(timeMsg[0], format='%Y-%m-%dT%H:%M:%S')
        timeObject += timedelta(hours=2)
        time.append(timeObject)

    if ("payload_hex:" in column):
        loraMsg = column.split('"')[1]
        payload = loraMsg.split(' ')
        try:
            if (payload[1] == "3"):
                level.append(int('0x'+payload[2], 0))
                voltage.append(int('0x'+payload[3], 0) / 10)
            else:
                time.pop()
        except:
            time.pop()


# plotting the points
fig, ax = plt.subplots()
# ax.plot(time, level)
ax.plot(time, voltage)

myFmt = mdates.DateFormatter('%d/%m - %H:%M')
ax.xaxis.set_major_formatter(myFmt)

plt.gcf().autofmt_xdate()

plt.xlabel('Time')
plt.ylabel('Charge Level [%]')
plt.title('Battery Level (Estimate)')
# plt.ylim(0, 101)
plt.ylim(10, 14)

# ax.set_facecolor('xkcd:grey')

# fig.patch.set_facecolor('xkcd:grey')
# function to show the plot
plt.show()
