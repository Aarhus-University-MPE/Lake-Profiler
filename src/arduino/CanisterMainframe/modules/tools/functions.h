union unpack {
  uint8_t i8;
  int i;
  char c;
  float f;
  unsigned int ui;
  unsigned long ul;
  long l;
  byte b[4];
};

enum PackageIdentifier {
  PACKAGE_ERROR          = 0,
  PACKAGE_ERROR_SENSOR   = 1,
  PACKAGE_ERROR_COMM_ACK = 2,
  PACKAGE_START          = 3,
  PACKAGE_STATUS         = 4,
  PACKAGE_0              = 10,
  PACKAGE_1              = 11,
  PACKAGE_2              = 12,
  PACKAGE_3              = 13,
  PACKAGE_4              = 14,
  PACKAGE_END            = 19
};

// Return integer value extracted from char array
long ExtractLongFromCharArray(char charArray[200], size_t index) {
  char tempChars[200];
  char *strtokIndx;
  strcpy(tempChars, charArray);
  long value = -1;

  // Scan forward to index, values separated by ","
  strtokIndx = strtok(tempChars, ",");
  for (size_t i = 0; i < index - 1; i++) {
    strtokIndx = strtok(NULL, ",");
  }

  value = atol(strtokIndx);

  return value;
}

// Return integer value extracted from char array
long ExtractLongFromCharArray(char charArray[200], size_t index, float scale) {
  char tempChars[200];
  char *strtokIndx;
  strcpy(tempChars, charArray);
  float value = 1.0f;
  long lValue = -1;

  // Scan forward to index, values separated by ","
  strtokIndx = strtok(tempChars, ",");
  for (size_t i = 0; i < index - 1; i++) {
    strtokIndx = strtok(NULL, ",");
  }

  value  = atof(strtokIndx);
  lValue = (long)(value * scale);

  return lValue;
}