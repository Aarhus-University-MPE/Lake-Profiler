union unpack {
  uint8_t i8;
  int i;
  char c;
  float f;
  unsigned int ui;
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