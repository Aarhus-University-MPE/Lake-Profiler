/************************************************************************/
/*																		*/
/*	RTCCI2C.cpp	--	Definition for RTCCI2C library 	    	            */
/*																		*/
/************************************************************************/
/*	Author:		Michelle Yu												*/
/*	Copyright 2011, Digilent Inc.										*/
/************************************************************************/
/*  File Description:													*/
/*		This file defines functions for RTCCI2C						    */
/*																		*/
/************************************************************************/
/*  Revision History:													*/
/*																		*/
/*	11/09/2011(MichelleY): created										*/
/*																		*/
/************************************************************************/

/* ------------------------------------------------------------ */
/*				Include File Definitions						*/
/* ------------------------------------------------------------ */
#include "RTCCI2C.h"

/* ------------------------------------------------------------ */
/*				Procedure Definitions							*/
/* ------------------------------------------------------------ *

/* ------------------------------------------------------------------- */
/** void RTCCI2C::begin()
**
**	Parameters:
**		None
**
**
**	Return Value:
**		None
**
**
**	Description:
**		This function setup I2C
*/
void RTCCI2C::begin() {
  Wire.begin();
}

// clock functions
/* ------------------------------------------------------------------- */
/** void RTCCI2C::startClock(void)
**
**	Parameters:
**		None
**
**
**	Return Value:
**		None
**
**
**	Description:
**		This function starts the RTCC_RTCC
*/
void RTCCI2C::startClock(void) {
  uint8_t temp = 0;

  // The 7th bit of the RTCC_RTCC second register controls the oscillator
  // Set address to the RTCC_RTCC second register
  Wire.beginTransmission(0x6F);
  Wire.write(0x00);
  Wire.endTransmission();

  Wire.requestFrom(0x6F, 1);

  // get second
  while (Wire.available()) {
    temp = Wire.read();
  }

  // set the 7th bit to stop the clock
  temp = (temp | 0x80);

  // start clock
  Wire.beginTransmission(0x6F);
  Wire.write(0x00);
  Wire.write(temp);
  Wire.endTransmission();
}
/* ------------------------------------------------------------------- */
/** void RTCCI2C::stopClock(void)
**
**	Parameters:
**		None
**
**
**	Return Value:
**		None
**
**
**	Description:
**		This function stops the RTCC_RTCC
*/
void RTCCI2C::stopClock(void) {
  uint8_t temp = 0;

  // The 7th bit of the RTCC_RTCC second register controls the oscillator
  // Set address to the RTCC_RTCC second register
  Wire.beginTransmission(0x6F);
  Wire.write(0x00);
  Wire.endTransmission();

  Wire.requestFrom(0x6F, 1);

  // get second
  while (Wire.available()) {
    temp = Wire.read();
  }

  // clear the 7th bit to stop the clock
  temp = (temp & 0x7F);

  // stop oscillator
  Wire.beginTransmission(0x6F);
  Wire.write(0x00);
  Wire.write(temp);
  Wire.endTransmission();
}

// alarm functions
/* ------------------------------------------------------------------- */
/** void RTCCI2C::enableAlarm(uint8_t dest, uint8_t config)
**
**	Parameters:
**		dest    - RTCC_ALM0 for alarm 0
**                RTCC_ALM1 for alarm 1
**      config  - alarm configuration:
**                RTCC_ALM_POL - MPF outputs high when alarm is triggered
**
**                NONE                       - seconds match
**                RTCC_ALMC0                 - minutes match
**                RTCC_ALMC1                 - hours match
**                RTCC_ALMC1 | ALMC0         - matches on day at midnight
**                RTCC_ALMC2                 - date match
**                RTCC_ALMC2 | ALMC1 | ALMC0 - seconds, minutes, hour, day, date,
**                                        and month match
**	Return Value:
**		None
**
**
**	Description:
**		This function enables and configures the dest alarm.
*/
void RTCCI2C::enableAlarm(uint8_t dest, uint8_t config) {
  uint8_t temp;
  uint8_t rgbAlmCfg[2];

  // Set address to the control register
  Wire.beginTransmission(0x6F);
  Wire.write(0x07);
  Wire.endTransmission();

  Wire.requestFrom(0x6F, 1);

  // get control bits
  while (Wire.available()) {
    temp = Wire.read();
  }

  // Set enable bit and alarm config register address
  // depending on the destination
  switch (dest) {
    case RTCC_ALM0:
      // set ALM0 enable bit
      temp = (temp | 0x10);
      // alarm config register address
      rgbAlmCfg[0] = 0x0D;
      break;
    case RTCC_ALM1:
      // set ALM1 enable bit
      temp = (temp | 0x20);
      // alarm config register address
      rgbAlmCfg[0] = 0x14;
      break;
    default:
      return;
  }

  // enable alarm control bit
  Wire.beginTransmission(0x6F);
  Wire.write(0x07);
  Wire.write(temp);
  Wire.endTransmission();

  // Set address to the alarm config/day register
  Wire.beginTransmission(0x6F);
  Wire.write(rgbAlmCfg[0]);
  Wire.endTransmission();

  Wire.requestFrom(0x6F, 1);

  // get day
  while (Wire.available()) {
    temp = Wire.read();
  }

  rgbAlmCfg[1] = ((temp & 0x07) | (config & 0xF0));

  // configure alarm
  Wire.beginTransmission(0x6F);
  Wire.write(rgbAlmCfg[0]);
  Wire.write(rgbAlmCfg[1]);
  Wire.endTransmission();
}
/* ------------------------------------------------------------------- */
/** void RTCCI2C::disableAlarm(uint8_t dest)
**
**	Parameters:
**		dest    - RTCC_ALM0 for alarm 0
**                RTCC_ALM1 for alarm 1
**
**	Return Value:
**		None
**
**
**	Description:
**		This function disables the dest alarm and clear the alarm interrupt
**  flag for that alarm.
*/
void RTCCI2C::disableAlarm(uint8_t dest) {
  uint8_t temp;

  // clear the alarm interrupt flag
  this->alarmOff(dest);

  // Set address to the control register
  Wire.beginTransmission(0x6F);
  Wire.write(0x07);
  Wire.endTransmission();

  Wire.requestFrom(0x6F, 1);

  // get control bits
  while (Wire.available()) {
    temp = Wire.read();
  }

  // Clear enable bit depending on the destination
  switch (dest) {
    case RTCC_ALM0:
      temp = (temp & 0xEF);
      break;
    case RTCC_ALM1:
      temp = (temp & 0xDF);
      break;
    default:
      return;
  }

  // configure alarm
  Wire.beginTransmission(0x6F);
  Wire.write(0x07);
  Wire.write(temp);
  Wire.endTransmission();
}
/* ------------------------------------------------------------------- */
/** void RTCCI2C::alarmOff(uint8_t dest)
**
**	Parameters:
**		dest    - RTCC_ALM0 for alarm 0
**                RTCC_ALM1 for alarm 1
**
**	Return Value:
**		None
**
**
**	Description:
**		This function clear the alarm interrupt flag for the dest alarm.
*/
void RTCCI2C::alarmOff(uint8_t dest) {
  uint8_t rgbFlag[2];
  uint8_t temp;

  // set alarm flag address depending on the destination
  switch (dest) {
    case RTCC_ALM0:
      rgbFlag[0] = 0x0D;
      break;
    case RTCC_ALM1:
      rgbFlag[0] = 0x14;
      break;
    default:
      return;
  }

  // Set address to the alarm config/day register
  Wire.beginTransmission(0x6F);
  Wire.write(rgbFlag[0]);
  Wire.endTransmission();

  Wire.requestFrom(0x6F, 1);

  while (Wire.available()) {
    temp = Wire.read();
  }

  // clear interrupt flag
  rgbFlag[1] = temp & 0xF7;

  Wire.beginTransmission(0x6F);
  Wire.write(rgbFlag[0]);
  Wire.write(rgbFlag[1]);
  Wire.endTransmission();
}
/* ------------------------------------------------------------------- */
/** unsigned int RTCCI2C::checkFlag(uint8_t src)
**
**	Parameters:
**		src    - RTCC_ALM0 for alarm 0
**               RTCC_ALM1 for alarm 1
**
**	Return Value:
**		unsigned int - 1 - alarm triggered
**                     0 - alarm not triggered
**
**
**	Description:
**		This function checks if the src alarm has been triggered.
*/
unsigned int RTCCI2C::checkFlag(uint8_t src) {
  uint8_t rgbFlag[2];
  uint8_t temp;

  // set alarm flag address depending on the destination
  switch (src) {
    case RTCC_ALM0:
      rgbFlag[0] = 0x0D;
      break;
    case RTCC_ALM1:
      rgbFlag[0] = 0x14;
      break;
    default:
      return 0;
  }

  // Set address to the alarm config/day register
  Wire.beginTransmission(0x6F);
  Wire.write(rgbFlag[0]);
  Wire.endTransmission();

  Wire.requestFrom(0x6F, 1);

  while (Wire.available()) {
    temp = Wire.read();
  }

  // check the for the Alarm Interrupt Flag
  if ((temp & 0x08) == 0x08)
    // alarm triggerd
    return 1;
  else
    // alarm not triggerd
    return 0;
}

// backup battery functions
/* ------------------------------------------------------------------- */
/** void RTCCI2C::enableVbat(void)
**
**	Parameters:
**		None
**
**	Return Value:
**		None
**
**	Description:
**		This function enables backup battery mode
*/
void RTCCI2C::enableVbat(void) {
  uint8_t temp;

  // The 3rd bit of the RTCC_RTCC day register controls VBATEN
  // Set address to the RTCC_RTCC day register
  Wire.beginTransmission(0x6F);
  Wire.write(0x03);
  Wire.endTransmission();

  Wire.requestFrom(0x6F, 1);

  // get day
  while (Wire.available()) {
    temp = Wire.read();
  }

  // set 3rd bit to enable backup battery mode
  temp = (temp | 0x08);

  // enable backup battery mode
  Wire.beginTransmission(0x6F);
  Wire.write(0x03);
  Wire.write(temp);
  Wire.endTransmission();
}
/* ------------------------------------------------------------------- */
/** void RTCCI2C::disableVbat(void)
**
**	Parameters:
**		None
**
**	Return Value:
**		None
**
**	Description:
**		This function disables backup battery mode
*/
void RTCCI2C::disableVbat(void) {
  uint8_t temp;

  // The 3rd bit of the RTCC_RTCC day register controls VBATEN
  // Set address to the RTCC_RTCC day register
  Wire.beginTransmission(0x6F);
  Wire.write(0x03);
  Wire.endTransmission();

  Wire.requestFrom(0x6F, 1);

  // get day
  while (Wire.available()) {
    temp = Wire.read();
  }

  // clear 3rd bit to disable backup battery mode
  temp = (temp & 0x37);

  // disable backup battery mode
  Wire.beginTransmission(0x6F);
  Wire.write(0x03);
  Wire.write(temp);
  Wire.endTransmission();
}

// get funcitons
/* ------------------------------------------------------------------- */
/** uint8_t RTCCI2C::getSec(uint8_t src)
**
**	Parameters:
**		src - RTCC_RTCC - real-time clock
**            RTCC_ALM0 - Alarm 0
**            RTCC_ALM1 - Alarm 1
**            ** RTCC_PWRU and RTCC_PWRD does not have the second parameter
**	Return Value:
**		uint8_t - second in hexadecimal
**
**	Description:
**		This function returns the second of the source
*/
uint8_t RTCCI2C::getSec(uint8_t src) {
  uint8_t bSecBuffer = 0;
  uint8_t bSec;

  // Set address of the second register depending on the source
  switch (src) {
    case RTCC_RTCC:
      bSec = 0x00;
      break;
    case RTCC_ALM0:
      bSec = 0x0A;
      break;
    case RTCC_ALM1:
      bSec = 0x11;
      break;
    default:
      return 0xFF;  // ERROR Invalid Source
  }

  // set address
  Wire.beginTransmission(0x6F);
  Wire.write(bSec);
  Wire.endTransmission();

  // read from address
  Wire.requestFrom(0x6F, 1);

  while (Wire.available()) {
    bSecBuffer = Wire.read();
  }

  // return valid bits
  return (bSecBuffer & 0x7F);
}
/* ------------------------------------------------------------------- */
/** uint8_t RTCCI2C::getMin(uint8_t src)
**
**	Parameters:
**		src - RTCC_RTCC - real-time clock
**            RTCC_ALM0 - Alarm 0
**            RTCC_ALM1 - Alarm 1
**            RTCC_PWRD - Power-down time-stamp
**            RTCC_PWRU - Power-up time-stamp
**
**	Return Value:
**		uint8_t - minute in hexadecimal
**
**	Description:
**		This function returns the minute of the source
*/
uint8_t RTCCI2C::getMin(uint8_t src) {
  uint8_t bMinBuffer = 0;
  uint8_t bMin;
  // Set address of the minute register depending on the source
  switch (src) {
    case RTCC_RTCC:
      bMin = 0x01;
      break;
    case RTCC_ALM0:
      bMin = 0x0B;
      break;
    case RTCC_ALM1:
      bMin = 0x12;
      break;
    case RTCC_PWRD:
      bMin = 0x18;
      break;
    case RTCC_PWRU:
      bMin = 0x1C;
      break;
    default:
      return 0xFF;  // ERROR Invalid Source
  }
  // set address
  Wire.beginTransmission(0x6F);
  Wire.write(bMin);
  Wire.endTransmission();

  // read from address
  Wire.requestFrom(0x6F, 1);

  while (Wire.available()) {
    bMinBuffer = Wire.read();
  }

  // return valid bits
  return (bMinBuffer & 0x7F);
}
/* ------------------------------------------------------------------- */
/** uint8_t RTCCI2C::getHour(uint8_t src)
**
**	Parameters:
**		src - RTCC_RTCC - real-time clock
**            RTCC_ALM0 - Alarm 0
**            RTCC_ALM1 - Alarm 1
**            RTCC_PWRD - Power-down time-stamp
**            RTCC_PWRU - Power-up time-stamp
**
**	Return Value:
**		uint8_t - hour in hexadecimal
**
**	Description:
**		This function returns the hour of the source
*/
uint8_t RTCCI2C::getHour(uint8_t src) {
  uint8_t bHourBuffer = 0;
  uint8_t bHour;
  // Set address of the hour register depending on the source
  switch (src) {
    case RTCC_RTCC:
      bHour = 0x02;
      break;
    case RTCC_ALM0:
      bHour = 0x0C;
      break;
    case RTCC_ALM1:
      bHour = 0x13;
      break;
    case RTCC_PWRD:
      bHour = 0x19;
      break;
    case RTCC_PWRU:
      bHour = 0x1D;
      break;
    default:
      return 0xFF;  // ERROR Invalid Source
  }
  // set address
  Wire.beginTransmission(0x6F);
  Wire.write(bHour);
  Wire.endTransmission();

  // read from address
  Wire.requestFrom(0x6F, 1);

  while (Wire.available()) {
    bHourBuffer = Wire.read();
  }

  // return valid bits
  // hour is in 12 hour format
  if ((bHourBuffer & 0x40) == 0x40) {
    return (bHourBuffer & 0x1F);
  }
  // hour is in 24 hour format
  else {
    return (bHourBuffer & 0x3F);
  }
}
/* ------------------------------------------------------------------- */
/** uint8_t RTCCI2C::getAmPm(uint8_t src)
**
**	Parameters:
**		src - RTCC_RTCC - real-time clock
**            RTCC_ALM0 - Alarm 0
**            RTCC_ALM1 - Alarm 1
**            RTCC_PWRD - Power-down time-stamp
**            RTCC_PWRU - Power-up time-stamp
**
**	Return Value:
**		uint8_t - 0 - RTCC_AM
**                1 - RTCC_PM
**
**	Description:
**		This function returns AM/PM for the source. This function should
** only be used when the hour is in 12 hour format.
*/
uint8_t RTCCI2C::getAmPm(uint8_t src) {
  uint8_t bHourBuffer = 0;
  uint8_t bHour;
  // Set address of the hour register depending on the source
  switch (src) {
    case RTCC_RTCC:
      bHour = 0x02;
      break;
    case RTCC_ALM0:
      bHour = 0x0C;
      break;
    case RTCC_ALM1:
      bHour = 0x13;
      break;
    case RTCC_PWRD:
      bHour = 0x19;
      break;
    case RTCC_PWRU:
      bHour = 0x1D;
      break;
    default:
      return 0xFF;  // ERROR Invalid Source
  }
  // set address
  Wire.beginTransmission(0x6F);
  Wire.write(bHour);
  Wire.endTransmission();

  // read from address
  Wire.requestFrom(0x6F, 1);
  while (Wire.available()) {
    bHourBuffer = Wire.read();
  }
  // return valid bits

  return (bHourBuffer & 0x20);
}
/* ------------------------------------------------------------------- */
/** uint8_t RTCCI2C::getDay(uint8_t src)
**
**	Parameters:
**		src - RTCC_RTCC - real-time clock
**            RTCC_ALM0 - Alarm 0
**            RTCC_ALM1 - Alarm 1
**            RTCC_PWRD - Power-down time-stamp
**            RTCC_PWRU - Power-up time-stamp
**
**	Return Value:
**		uint8_t - day in hexadecimal
**
**	Description:
**		This function returns the day of the source
*/
uint8_t RTCCI2C::getDay(uint8_t src) {
  uint8_t bDayBuffer = 0;
  uint8_t bDay;
  // Set address of the day register depending on the source
  switch (src) {
    case RTCC_RTCC:
      bDay = 0x03;
      break;
    case RTCC_ALM0:
      bDay = 0x0D;
      break;
    case RTCC_ALM1:
      bDay = 0x14;
      break;
    case RTCC_PWRD:
      bDay = 0x1B;
      break;
    case RTCC_PWRU:
      bDay = 0x1F;
      break;
    default:
      return 0xFF;  // ERROR Invalid Source
  }
  // set address
  Wire.beginTransmission(0x6F);
  Wire.write(bDay);
  Wire.endTransmission();

  // read from address
  Wire.requestFrom(0x6F, 1);

  while (Wire.available()) {
    bDayBuffer = Wire.read();
  }

  // return valid bits
  if (src == RTCC_PWRD || src == RTCC_PWRU) {
    return ((bDayBuffer & 0xE0) >> 5);
  } else {
    return (bDayBuffer & 0x07);
  }
}
/* ------------------------------------------------------------------- */
/** uint8_t RTCCI2C::getDate(uint8_t src)
**
**	Parameters:
**		src - RTCC_RTCC - real-time clock
**            RTCC_ALM0 - Alarm 0
**            RTCC_ALM1 - Alarm 1
**            RTCC_PWRD - Power-down time-stamp
**            RTCC_PWRU - Power-up time-stamp
**
**	Return Value:
**		uint8_t - date in hexadecimal
**
**	Description:
**		This function returns the date of the source
*/
uint8_t RTCCI2C::getDate(uint8_t src) {
  uint8_t bDateBuffer = 0;
  uint8_t bDate;
  // Set address of the date register depending on the source
  switch (src) {
    case RTCC_RTCC:
      bDate = 0x04;
      break;
    case RTCC_ALM0:
      bDate = 0x0E;
      break;
    case RTCC_ALM1:
      bDate = 0x15;
      break;
    case RTCC_PWRD:
      bDate = 0x1A;
      break;
    case RTCC_PWRU:
      bDate = 0x1E;
      break;
    default:
      return 0xFF;  // ERROR Invalid Source
  }
  // set address
  Wire.beginTransmission(0x6F);
  Wire.write(bDate);
  Wire.endTransmission();

  // read from address
  Wire.requestFrom(0x6F, 1);

  while (Wire.available()) {
    bDateBuffer = Wire.read();
  }

  // return valid bits
  return (bDateBuffer & 0x3F);
}
/* ------------------------------------------------------------------- */
/** uint8_t RTCCI2C::getMonth(uint8_t src)
**
**	Parameters:
**		src - RTCC_RTCC - real-time clock
**            RTCC_ALM0 - Alarm 0
**            RTCC_ALM1 - Alarm 1
**            RTCC_PWRD - Power-down time-stamp
**            RTCC_PWRU - Power-up time-stamp
**
**	Return Value:
**		uint8_t - month in hexadecimal
**
**	Description:
**		This function returns the month of the source
*/
uint8_t RTCCI2C::getMonth(uint8_t src) {
  uint8_t bMonthBuffer = 0;
  uint8_t bMonth;
  // Set address of the month register depending on the source
  switch (src) {
    case RTCC_RTCC:
      bMonth = 0x05;
      break;
    case RTCC_ALM0:
      bMonth = 0x0F;
      break;
    case RTCC_ALM1:
      bMonth = 0x16;
      break;
    case RTCC_PWRD:
      bMonth = 0x1B;
      break;
    case RTCC_PWRU:
      bMonth = 0x1F;
      break;
    default:
      return 0xFF;  // ERROR Invalid Source
  }
  // set address
  Wire.beginTransmission(0x6F);
  Wire.write(bMonth);
  Wire.endTransmission();

  // read from address
  Wire.requestFrom(0x6F, 1);

  while (Wire.available()) {
    bMonthBuffer = Wire.read();
  }

  // return valid bits
  return (bMonthBuffer & 0x1F);
}
/* ------------------------------------------------------------------- */
/** uint8_t RTCCI2C::getYear()
**
**	Parameters:
**		None
**
**	Return Value:
**		uint8_t - year in hexadecimal
**
**	Description:
**		This function returns the year of the RTCC_RTCC.
** The year parameter is only available for the RTCC_RTCC.
*/
uint8_t RTCCI2C::getYear() {
  uint8_t bYearBuffer = 0;

  // set address
  //  only RTCC_RTCC has the year register
  Wire.beginTransmission(0x6F);
  Wire.write(0x06);
  Wire.endTransmission();

  // read from address
  Wire.requestFrom(0x6F, 1);

  while (Wire.available()) {
    bYearBuffer = Wire.read();
  }

  // return valid bits
  return (bYearBuffer & 0xFF);
}

// set function
/* ------------------------------------------------------------------- */
/** void RTCCI2C::setSec(uint8_t dest, uint8_t value)
**
**	Parameters:
**		dest    - RTCC_RTCC - real-time clock
**                RTCC_ALM0 - Alarm 0
**                RTCC_ALM1 - Alarm 1
**      value   - desire value for the second in HEX 0x00-0x59
**
**	Return Value:
**		None
**
**	Description:
**		This function sets the second of the dest with the value.
*/
void RTCCI2C::setSec(uint8_t dest, uint8_t value) {
  uint8_t rgbSec[2];
  rgbSec[1] = value;
  uint8_t temp;

  // validate second 0x00-0x59
  if (value < 0x60) {
    // Set address of the second register depending on the destination
    switch (dest) {
      case RTCC_RTCC:
        rgbSec[0] = 0x00;
        // set address
        Wire.beginTransmission(0x6F);
        Wire.write(rgbSec[0]);
        Wire.endTransmission();

        // read from address
        Wire.requestFrom(0x6F, 1);

        while (Wire.available()) {
          temp = Wire.read();
        }
        if ((temp & 0x80) == 0x80) {  // preserve configuration bits
          rgbSec[1] = (rgbSec[1] | 0x80);
        }
        break;
      case RTCC_ALM0:
        rgbSec[0] = 0x0A;
        break;
      case RTCC_ALM1:
        rgbSec[0] = 0x11;
        break;
      default:  // Should not enter
        return;
    }

    // set second
    Wire.beginTransmission(0x6F);
    Wire.write(rgbSec[0]);
    Wire.write(rgbSec[1]);
    Wire.endTransmission();
  }
}
/* ------------------------------------------------------------------- */
/** void RTCCI2C::setMin(uint8_t dest, uint8_t value)
**
**	Parameters:
**		dest    - RTCC_RTCC - real-time clock
**                RTCC_ALM0 - Alarm 0
**                RTCC_ALM1 - Alarm 1
**      value   - desire value for the minute in HEX 0x00-0x59
**
**	Return Value:
**		None
**
**	Description:
**		This function sets the minute of the dest with the value.
*/
void RTCCI2C::setMin(uint8_t dest, uint8_t value) {
  uint8_t rgbMin[2];
  rgbMin[1] = value;

  // validate minute 0x00-0x59
  if (value < 0x60) {
    // Set address of the minute register depending on the destination
    switch (dest) {
      case RTCC_RTCC:
        rgbMin[0] = 0x01;
        break;
      case RTCC_ALM0:
        rgbMin[0] = 0x0B;
        break;
      case RTCC_ALM1:
        rgbMin[0] = 0x12;
        break;
      default:  // Should not enter
        return;
    }
    // set minute
    Wire.beginTransmission(0x6F);
    Wire.write(rgbMin[0]);
    Wire.write(rgbMin[1]);
    Wire.endTransmission();
  }
}
/* ------------------------------------------------------------------- */
/** void RTCCI2C::setHour(uint8_t dest, uint8_t value, uint8_t ampm)
**
**	Parameters:
**		dest    - RTCC_RTCC - real-time clock
**                RTCC_ALM0 - Alarm 0
**                RTCC_ALM1 - Alarm 1
**      value   - desire value for the hour in 12 hour format.
**                the value should be represented in HEX and should be
**                between 0x01-0x12
**      ampm  -  RTCC_AM
**               RTCC_PM
**
**	Return Value:
**		None
**
**	Description:
**	    This function sets the hour of the dest with the value in
**  12 hour format.
*/
void RTCCI2C::setHour(uint8_t dest, uint8_t value, uint8_t ampm) {
  uint8_t rgbHour[2];
  uint8_t temp;

  // validate hour 0x01-0x12
  if (value < 0x13 && value > 0) {
    // Set address of the hour register depending on the destination
    switch (dest) {
      case RTCC_RTCC:
        rgbHour[0] = 0x02;
        break;
      case RTCC_ALM0:
        rgbHour[0] = 0x0C;
        break;
      case RTCC_ALM1:
        rgbHour[0] = 0x13;
        break;
      default:  // Should not enter
        return;
    }

    // set the format bit to 12 hr format
    // set ampm bit if PM
    if (ampm == RTCC_PM) {
      rgbHour[1] = (value | 0x60);
    } else {
      rgbHour[1] = (value | 0x40);
    }

    // set hour
    Wire.beginTransmission(0x6F);
    Wire.write(rgbHour[0]);
    Wire.write(rgbHour[1]);
    Wire.endTransmission();
  }
}
/* ------------------------------------------------------------------- */
/** void RTCCI2C::setHour(uint8_t dest, uint8_t value)
**
**	Parameters:
**		dest    - RTCC_RTCC - real-time clock
**                RTCC_ALM0 - Alarm 0
**                RTCC_ALM1 - Alarm 1
**      value   - desire value for the hour in 24 hour format.
**                the value should be represented in HEX and should be
**                between 0x00-0x23
**
**	Return Value:
**		None
**
**	Description:
**  This function sets the hour of the destination with the value
**  in 24 hour format.
*/
void RTCCI2C::setHour(uint8_t dest, uint8_t value) {
  uint8_t rgbHour[2];
  uint8_t temp;

  // validate hour 0x00-0x23
  if (value < 0x24) {
    // Set address of the hour register depending on the destination
    switch (dest) {
      case RTCC_RTCC:
        rgbHour[0] = 0x02;
        break;
      case RTCC_ALM0:
        rgbHour[0] = 0x0C;
        break;
      case RTCC_ALM1:
        rgbHour[0] = 0x13;
        break;
      default:  // Should not enter
        return;
    }

    rgbHour[1] = value;

    // set hour
    Wire.beginTransmission(0x6F);
    Wire.write(rgbHour[0]);
    Wire.write(rgbHour[1]);
    Wire.endTransmission();
  }
}
/* ------------------------------------------------------------------- */
/** void RTCCI2C::setDay(uint8_t dest, uint8_t value)
**
**	Parameters:
**		dest    - RTCC_RTCC - real-time clock
**                RTCC_ALM0 - Alarm 0
**                RTCC_ALM1 - Alarm 1
**      value   - desire value for the day in HEX 0x01-0x07
**
**	Return Value:
**		None
**
**	Description:
**		This function sets the day of the dest with the value.
*/
void RTCCI2C::setDay(uint8_t dest, uint8_t value) {
  uint8_t rgbDay[2];
  uint8_t temp;

  // validate day 0x01-0x07
  if (value < 0x08 && value > 0) {
    // Set address of the day register depending on the destination
    switch (dest) {
      case RTCC_RTCC:
        rgbDay[0] = 0x03;
        break;
      case RTCC_ALM0:
        rgbDay[0] = 0x0D;
        break;
      case RTCC_ALM1:
        rgbDay[0] = 0x14;
        break;
      default:  // Should not enter
        return;
    }

    // set address
    Wire.beginTransmission(0x6F);
    Wire.write(rgbDay[0]);
    Wire.endTransmission();

    // read from address
    Wire.requestFrom(0x6F, 1);

    while (Wire.available()) {
      temp = Wire.read();
    }

    // reserve the control bits
    rgbDay[1] = ((value & 0x07) | (temp & 0xF8));

    // set day
    Wire.beginTransmission(0x6F);
    Wire.write(rgbDay[0]);
    Wire.write(rgbDay[1]);
    Wire.endTransmission();
  }
}
/* ------------------------------------------------------------------- */
/** void RTCCI2C::setDate(uint8_t dest, uint8_t value)
**
**	Parameters:
**		dest    - RTCC_RTCC - real-time clock
**                RTCC_ALM0 - Alarm 0
**                RTCC_ALM1 - Alarm 1
**      value   - desire value for the date in HEX 0x01-0x31
**
**	Return Value:
**		None
**
**	Description:
**		This function sets the date of the dest with the value.
*/
void RTCCI2C::setDate(uint8_t dest, uint8_t value) {
  uint8_t rgbDate[2];
  rgbDate[1] = value;

  // validate date 0x01-0x31
  if (value < 0x32 && value > 0) {
    // Set address of the date register depending on the destination
    switch (dest) {
      case RTCC_RTCC:
        rgbDate[0] = 0x04;
        break;
      case RTCC_ALM0:
        rgbDate[0] = 0x0E;
        break;
      case RTCC_ALM1:
        rgbDate[0] = 0x15;
        break;
      default:  // Should not enter
        return;
    }
    // set date
    Wire.beginTransmission(0x6F);
    Wire.write(rgbDate[0]);
    Wire.write(rgbDate[1]);
    Wire.endTransmission();
  }
}
/* ------------------------------------------------------------------- */
/** void RTCCI2C::setMonth(uint8_t dest, uint8_t value)
**
**	Parameters:
**		dest    - RTCC_RTCC - real-time clock
**                RTCC_ALM0 - Alarm 0
**                RTCC_ALM1 - Alarm 1
**      value   - desire value for the month in HEX 0x01-0x12
**
**	Return Value:
**		None
**
**	Description:
**		This function sets the month of the dest with the value.
*/
void RTCCI2C::setMonth(uint8_t dest, uint8_t value) {
  uint8_t rgbMonth[2];
  rgbMonth[1] = value;

  // validate month 0x01-0x12
  if (value < 0x13 && value > 0) {
    // Set address of the month register depending on the destination
    switch (dest) {
      case RTCC_RTCC:
        rgbMonth[0] = 0x05;
        break;
      case RTCC_ALM0:
        rgbMonth[0] = 0x0F;
        break;
      case RTCC_ALM1:
        rgbMonth[0] = 0x16;
        break;
      default:  // Should not enter
        return;
    }
    // set month
    Wire.beginTransmission(0x6F);
    Wire.write(rgbMonth[0]);
    Wire.write(rgbMonth[1]);
    Wire.endTransmission();
  }
}
/* ------------------------------------------------------------------- */
/** void RTCCI2C::setYear(uint8_t value)
**
**	Parameters:
**      value   - desire value for the month in HEX 0x00-0x99
**
**	Return Value:
**		None
**
**	Description:
**		This function sets the year of the RTCC_RTCC with the value.
**  The year parameter is only available for the RTCC_RTCC.
*/
void RTCCI2C::setYear(uint8_t value) {
  uint8_t rgbYear[2];
  rgbYear[1] = value;
  rgbYear[0] = 0x06;
  // validate year 0x00-0x99
  if (value < 0xA0) {
    // set month
    Wire.beginTransmission(0x6F);
    Wire.write(rgbYear[0]);
    Wire.write(rgbYear[1]);
    Wire.endTransmission();
  }
}
