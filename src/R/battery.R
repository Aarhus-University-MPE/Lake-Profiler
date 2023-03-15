library(tidyverse)
library(lubridate)
library(httr)
library(readr)
library(gridExtra)

# Define duration going back
duration <- "3d"

# Enable Legacy payloads
legacy <- FALSE

# Specify the output location for saving battery information as battery.csv

directory <- "C:/Users/au291035/Dropbox/docs/OrmstrupSø/profiling_buoy/battery.csv"

# Raw payload directory
url <- "https://prototype.asap-forecast.com/logs/ormstrup-au?filter=ttn-payload&back=3d"


download.file(url, destfile = directory, method = "curl", extra = "-k")

# Import data- battery.csv
data <- read.csv(directory, check.names=FALSE, quote = "\"", header=TRUE)

# Initialize variables
level <- vector()
voltage <- vector()
time <- vector()

# Extract payloads containing battery readings
for (col in colnames(data)) {
  # Get LoRa message timestamp
  if (grepl("createdAt", col)) {
    loraMsg <- ymd_hms(col)
    time <- c(time, loraMsg)
  }
  
  # Payload data
  if (grepl("payload_hex:", col)) {
    loraMsg <- str_split(col, ":")[[1]][2]
    
    payload <- str_split(loraMsg, " ")
  #  print(payload)
    # Battery reading message identifier
    if (payload[[1]][2] == "3") {

      level <- c(level, as.integer(paste0("0x", payload[[1]][3]), 0))
     # print(str_extract_all(payload[[1]][4], "[[:alnum:]]+"))
     if(legacy){
      voltage <- c(voltage, as.integer(paste0("0x", str_extract_all(payload[[1]][4], "[[:alnum:]]+")), 0) / 10)
     } else{
      print(as.integer(paste0("0x", str_extract_all(payload[[1]][4], "[[:alnum:]]+")), 0) / 10)
     }
    } else {
      # Message was not battery reading, remove last payload time
      time <- time[-length(time)]
    }
  }
}
time <- as.POSIXct(time, origin = "1970-01-01", tz = "UTC")
time <- lubridate::ymd_hms(time)

# Plotting battery readings
df <- data.frame(time = time, level = level, voltage = voltage)
p1 <- ggplot(df, aes(x = time)) +
  geom_line(aes(y = level), color = "black") +
  scale_y_continuous(name = "Charge Level [%]", limits = c(0, 100)) +

  labs(title = "Battery Level (Estimate)", x = "Time") +
  theme_classic()

# Format x-axis
p1 <- p1 + scale_x_datetime(date_labels = "%d/%m - %H:%M", date_breaks = "6 hour") +
  theme(axis.text.x = element_text(angle = 45, vjust = 1, hjust = 1))

p2 <- ggplot(df, aes(x = time)) +
  geom_line(aes(y = voltage), color = "black") +

  labs(title = "Battery Voltage", x = "Time") +
  theme_classic()

# Format x-axis
p2 <- p2 + scale_x_datetime(date_labels = "%d/%m - %H:%M", date_breaks = "6 hour") +
  theme(axis.text.x = element_text(angle = 45, vjust = 1, hjust = 1))

# Show plot

grid.arrange(p1, p2, nrow = 1)

