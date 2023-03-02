from datetime import timedelta
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.dates as mdates
import os

bashCommand = "curl -o \"D:/Projects/Lake-Profiler/data/LoRa/payload.csv\" \"https://prototype.asap-forecast.com/logs/ormstrup-au?filter=ttn-payload&back="
bashCommand += "1d\""

os.system(bashCommand)

data = pd.read_csv('./data/LoRa/payload.csv')

payloadData = []
voltage = []
time = []


def parsePayload(payload):
    # Payload Index
    index = parsePayloadInt(payload, 2)

    ch4ConcentrationEstimate = parsePayloadInt(payload, 7)
    ch4Concentration = parsePayloadInt(payload, 11)
    co2RawValues = parsePayloadInt(payload, 15)
    co2Concentration = parsePayloadInt(payload, 19)
    depth = parsePayloadInt(payload, 23)
    temperatures = parsePayloadInt(payload, 27)
    luminesence = parsePayloadInt(payload, 31, 2)

    sampleID = parsePayloadInt(payload, 34, 2)
    timeStamp = parsePayloadInt(payload, 36)

    data = [index, ch4ConcentrationEstimate, ch4Concentration,
            co2RawValues, co2Concentration, depth, temperatures, luminesence, sampleID, timeStamp]

    return data


def parsePayloadInt(payload, startIndex, length=4):
    # Add hex identifier
    byteString = ['0x' + s for s in payload[startIndex:startIndex+length]]
    # print(byteString)

    # Convert to byteframe
    frame = b""
    for s in byteString:
        frame += bytes(int(s, 16).to_bytes(1, "little"))

    # Convert to integer values
    intValue = int.from_bytes(frame, "little", signed=True)

    return intValue


payloadDataFormatted = [["Index", "CH4 Estimate", "CH4 Concentration",
                         "CO2 Raw", "CO2 Concentration", "Depth", "Temperatures", "Luminesence", "Sample ID", "Timestamp"]]

payloadDataFormatted.append(["-", "ppm x 1000", "ppm x 1000",
                             "ppm x 1000", "ppmv x 1000", "m x 1000", "deg C x 1000", "-", "-", "Unix ms"])

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
            if (payload[1] == "41"):
                payloadData.append(payload)
                formattedData = parsePayload(payload)
                # check for duplicates
                if (formattedData[0] != payloadDataFormatted[payloadDataFormatted.__len__()-1][0]):
                    payloadDataFormatted.append(formattedData)
                # level.append(int('0x'+payload[2], 0))
                # voltage.append(int('0x'+payload[3], 0) / 10)
            else:
                time.pop()
        except:
            time.pop()

# print(payloadDataFormatted)

dataArray = np.asarray(payloadData)
pd.DataFrame(dataArray).to_csv('./data/LoRa/payloadExtract.csv', sep=';')

dataArrayFormatted = np.asarray(payloadDataFormatted)
pd.DataFrame(dataArrayFormatted).to_csv(
    './data/LoRa/payloadExtractFormatted.csv', sep=';')
