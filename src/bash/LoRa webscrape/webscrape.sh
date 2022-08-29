#!/bin/bash
#web scraping lora data website

echo "Downloading LoRa data"
$curl -o "D:\Projects\Lake-Profiler\data\LoRa\battery.csv" "https://prototype.asap-forecast.com/logs/ormstrup-au?filter=ttn-payload&back=12h"
