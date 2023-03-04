from datetime import timedelta
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.dates as mdates
import os

# Define duration going back
duration = "5d"

# Raw payload directory
directory = os.getcwd() + "/data/LoRa/payload.csv"

# Bash command to download full payload for specified duration
bashCommand = "curl -o \"" + directory + \
    "\" \"https://prototype.asap-forecast.com/logs/ormstrup-au?filter=ttn-payload&back=" + duration + "\""

# Execute bash command
os.system(bashCommand)

# Import data
data = pd.read_csv('./data/LoRa/payload.csv')


# Function to parse a payload message
def parsePayload(payload):
    # Payload Index
    index = parsePayloadInt(payload, 2)

    # Sensor data
    ch4ConcentrationEstimate = parsePayloadInt(payload, 7)
    ch4Concentration = parsePayloadInt(payload, 11)
    co2RawValues = parsePayloadInt(payload, 15)
    co2Concentration = parsePayloadInt(payload, 19)
    depth = parsePayloadInt(payload, 23)
    temperatures = parsePayloadInt(payload, 27)
    luminesence = parsePayloadInt(payload, 31, 2)

    # Sample data
    sampleID = parsePayloadInt(payload, 34, 2)
    timeStamp = parsePayloadInt(payload, 36)

    # Bundle data
    data = [index, ch4ConcentrationEstimate, ch4Concentration,
            co2RawValues, co2Concentration, depth, temperatures, luminesence, sampleID, timeStamp]

    return data


# Extract integer from payload
def parsePayloadInt(payload, startIndex, length=4):
    # Add hex identifier
    byteString = ['0x' + s for s in payload[startIndex:startIndex+length]]

    # Convert to byteframe
    frame = b""
    for s in byteString:
        frame += bytes(int(s, 16).to_bytes(1, "little"))

    # Convert to integer values
    intValue = int.from_bytes(frame, "little", signed=True)

    return intValue


# Initialize csv headers
payloadDataFormatted = [["Index", "CH4 Estimate", "CH4 Concentration",
                         "CO2 Raw", "CO2 Concentration", "Depth", "Temperatures", "Luminesence", "Sample ID", "Timestamp"]]

payloadDataFormatted.append(["-", "ppm x 1000", "ppm x 1000",
                             "ppm x 1000", "ppmv x 1000", "m x 1000", "deg C x 1000", "-", "-", "Unix ms"])

# Initialize data arrays
time = []
payloadData = []

# Extract payloads containing profiling packages
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
            # Profiling payload message identifier
            if (payload[1] == "41"):
                # Extract and format payload
                payloadData.append(payload)
                formattedData = parsePayload(payload)
                # check for duplicates
                if (formattedData[0] != payloadDataFormatted[payloadDataFormatted.__len__()-1][0]):
                    payloadDataFormatted.append(formattedData)

            # Message was not Profiling payload, remove last payload time
            else:
                time.pop()
        # Empty data package, remove last payload time
        except:
            time.pop()


# Convert to numpy array
dataArray = np.asarray(payloadData)
dataArrayFormatted = np.asarray(payloadDataFormatted)

# Convert to csv file and save
pd.DataFrame(dataArray).to_csv('./data/LoRa/payloadExtract.csv', sep=';')
pd.DataFrame(dataArrayFormatted).to_csv(
    './data/LoRa/payloadExtractFormatted.csv', sep=';')
