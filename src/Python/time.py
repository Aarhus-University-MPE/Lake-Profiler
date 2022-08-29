import pandas as pd

#
msg = "2022-08-24T19:51:36.117Z"
timeMsg = msg.split('.')

print(pd.to_datetime(timeMsg[0], format='%Y-%m-%dT%H:%M:%S'))
