library(tidyverse)
library(lubridate)
library(httr)
library(readr)
library(gridExtra)

# Define duration going back -
duration <- "2d"


# Enable Legacy payloads
legacy <- FALSE

# Specify the output location for saving battery information as battery.csv

directory <- "C:/Users/au291035/Dropbox/profiling_buoy/data/battery.csv"


# Raw payload directory
url <- paste0("https://prototype.asap-forecast.com/logs/ormstrup-au?filter=ttn-payload&back=", duration)


download.file(url, destfile = directory, method = "curl", extra = "-k")

# Import data- battery.csv
data <- read.csv(directory, check.names=FALSE, quote = "\"", header=FALSE)

# Initialize variables
level <- vector()
voltage <- vector()
time <- vector()



# Initialize variables
level <- vector()
voltage <- vector()
time <- vector()

# Extract payloads containing battery readings
for (col in 1:ncol(data)) {
  # Get LoRa message timestamp
  if (grepl("createdAt", data[,col])) {
    loraMsg <- ymd_hms(data[,col])
    time <- c(time, loraMsg)
  }
  
  # Extract payload data
  if (grepl("payload_hex:", data[,col])) {
    loraMsg <- str_split(data[,col], ":")[[1]][2]
    payload <- str_split(loraMsg, " ")
    
    if (is.na(payload) || any(payload == "NA")) {
      # Remove the last timestamp if payload is NA
      time <- time[-length(time)]
    } else {
      tryCatch({
        # Battery reading message identifier
        if (payload[[1]][2] == "3") {
          level <- c(level, as.integer(paste0("0x", payload[[1]][3]), 0))
          if (legacy) {
            voltage <- c(voltage, as.integer(paste0("0x", str_extract_all(payload[[1]][4], "[[:alnum:]]+")), 0) / 10)
          } else {
            voltage <- c(voltage, as.integer(paste0("0x", str_extract_all(payload[[1]][4], "[[:alnum:]]+"),
                                                    str_extract_all(payload[[1]][5], "[[:alnum:]]+")))/100)
          }
        }
        else{
          # Message was not battery reading, remove last payload time
          time <- time[-length(time)]
        }
      }, error = function(e) {
        # handle the error
        message("An error occurred:", conditionMessage(e))
        # Remove the last timestamp if payload contains "NA"
        if (any(payload == "NA")) {
          time <- time[-length(time)]
        }
      })
    }
  }
}



time <- as.POSIXct(time, origin = "1970-01-01", tz = "UTC")
time <- lubridate::ymd_hms(time)

# Plotting battery readings
df <- data.frame(time = time, level = level, voltage = voltage)
p1 <- ggplot(df, aes(x = time)) +
  geom_line(aes(y = level), color = "blue", lwd=1) +
  scale_y_continuous(name = "Charge Level [%]", limits = c(0, 100)) +

  labs(title = "Battery Level (Estimate)", x = "Time") 

# Format x-axis
#p1 <- p1 + scale_x_datetime(date_labels = "%d/%m - %H:%M", date_breaks = "6 hour") +
#  theme(axis.text.x = element_text(angle = 45, vjust = 1, hjust = 1))

p2 <- ggplot(df, aes(x = time)) +
  geom_line(aes(y = voltage), color = "red", lwd=1) +
  scale_y_continuous(name = "Voltage", limits = c(0, 15)) +
  labs(title = "Battery Voltage", x = "Time") 
# Format x-axis
#p2 <- p2 + scale_x_datetime(date_labels = "%d/%m - %H:%M", date_breaks = "6 hour") +
#  theme(axis.text.x = element_text(angle = 45, vjust = 1, hjust = 1))

# Show plot

grid.arrange(p1, p2, nrow = 1)


