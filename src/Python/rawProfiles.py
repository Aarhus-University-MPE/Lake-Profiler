import pandas as pd
import numpy as np

# Raw payload directory
dataDirectory = "C:/Users/madsr/Desktop/23042012.csv"

# Import data
data = pd.read_csv(dataDirectory, header=None)


# Function to parse a payload message
def parsePayload(payload):
    # Payload Index
    index = parsePayloadInt(payload, 1)

    # Sensor data
    ch4ConcentrationEstimate = parsePayloadInt(payload, 6)
    ch4Concentration = parsePayloadInt(payload, 10)
    co2RawValues = parsePayloadInt(payload, 14)
    co2Concentration = parsePayloadInt(payload, 18)
    depth = parsePayloadInt(payload, 22)
    temperatures = parsePayloadInt(payload, 26)
    luminesence = parsePayloadInt(payload, 31, 1)

    # Sample data
    sampleID = parsePayloadInt(payload, 33, 2)

    # Bundle data
    data = [index, ch4ConcentrationEstimate, ch4Concentration,
            co2RawValues, co2Concentration, depth, temperatures, luminesence, sampleID]

    return data


# Extract integer from payload
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


# Initialize csv headers
payloadDataFormatted = [["Index", "CH4 Estimate", "CH4 Concentration",
                         "CO2 Raw", "CO2 Concentration", "Depth", "Temperatures", "Luminesence", "Sample ID"]]

payloadDataFormatted.append(["-", "ppm x 1000", "ppm x 1000",
                             "ppm x 1000", "ppmv x 1000", "m x 1000", "deg C x 1000", "-", "-"])

# Initialize data arrays
time = []
payloadData = []


# Extract payloads containing profiling packages
for i in range(len(data)):
    try:
        payload = data.iloc[i].values[0].split(" ")

        # Extract and format payload
        payloadData.append(payload)
        formattedData = parsePayload(payload)
        # check for duplicates
        if (formattedData[0] != payloadDataFormatted[payloadDataFormatted.__len__()-1][0]):
            payloadDataFormatted.append(formattedData)
    except:
        pass


# Convert to numpy array
dataArray = np.asarray(payloadData)
dataArrayFormatted = np.asarray(payloadDataFormatted)

# Convert to csv file and save
pd.DataFrame(dataArray).to_csv('./data/LoRa/payloadExtract.csv', sep=';')
pd.DataFrame(dataArrayFormatted).to_csv(
    './data/LoRa/payloadExtractFormatted.csv', sep=';')
