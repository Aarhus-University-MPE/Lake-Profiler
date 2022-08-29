import os
# -o \"D:\Projects\Lake-Profiler\data\LoRa\battery.csv\"

bashCommand = "curl -o \"C:/Users/madsr/Desktop/output.txt\" \"https://prototype.asap-forecast.com/logs/ormstrup-au?filter=ttn-payload&back=1h\""
# print(bashCommand)

os.system(bashCommand)
