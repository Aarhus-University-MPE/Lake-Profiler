from datetime import timedelta
import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.dates as mdates
import os

bashCommand = "curl -o \"D:/Projects/Lake-Profiler/data/LoRa/battery.csv\" \"https://prototype.asap-forecast.com/logs/ormstrup-au?filter=ttn-payload&back="
bashCommand += "25h\""

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

        level.append(int('0x'+payload[2], 0))
        voltage.append(int('0x'+payload[3], 0) / 10)


# plotting the points
fig, ax = plt.subplots()
ax.plot(time, level)

myFmt = mdates.DateFormatter('%H:%M')
ax.xaxis.set_major_formatter(myFmt)

plt.gcf().autofmt_xdate()

# naming the x axis
plt.xlabel('Time')
# naming the y axis
plt.ylabel('Charge Level [%]')
# giving a title to my graph
plt.title('Battery Level (Estimate)')

# function to show the plot
plt.show()


# plotting the points
fig, ax = plt.subplots()
ax.plot(time, voltage)

myFmt = mdates.DateFormatter('%H:%M')
ax.xaxis.set_major_formatter(myFmt)

plt.gcf().autofmt_xdate()

# naming the x axis
plt.xlabel('Time')
# naming the y axis
plt.ylabel('Battery Voltage [V]')
# giving a title to my graph
plt.title('Battery Level (Estimate)')

# function to show the plot
plt.show()
