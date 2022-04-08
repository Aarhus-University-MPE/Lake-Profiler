/************************************************************************/
/*																		*/
/*	RTCCI2C.h	--	Declaration for RTCCI2C library 	    			*/
/*																		*/
/************************************************************************/
/*	Author:		Michelle Yu												*/
/*	Copyright 2011, Digilent Inc.										*/
/************************************************************************/
/*  File Description:													*/
/*		This file declares functions for RTCCI2C	    				*/
/*																		*/
/************************************************************************/
/*  Revision History:													*/
/*																		*/
/*	11/09/2011(MichelleY): created										*/
/*																		*/
/************************************************************************/
#if !defined(RTCCI2C_H)
#define RTCCI2C_H


/* ------------------------------------------------------------ */
/*				Include File Definitions						*/
/* ------------------------------------------------------------ */
#include <inttypes.h>
#include <Wire.h>

/* ------------------------------------------------------------ */
/*					Procedure Declarations						*/
/* ------------------------------------------------------------ */
//available source/destination
#define RTCC_RTCC    0x01    // real- time clock
#define RTCC_ALM0    0x02    // alarm 0
#define RTCC_ALM1    0x03    // alarm 1
#define RTCC_PWRD    0x04    // power-down time-stamp
#define RTCC_PWRU    0x05    // power-up time-stamp

//alarm configuration bits
#define RTCC_ALM_POL 0x80
#define RTCC_ALMC2  0x40
#define RTCC_ALMC1  0x20
#define RTCC_ALMC0  0x10

//AM/PM
#define RTCC_AM 0
#define RTCC_PM 1

class RTCCI2C {
public:

  //initialize 
  void begin();
  
  //clock functions
  void startClock(void);
  void stopClock(void);
  
  //alarm functions
  void enableAlarm(uint8_t dest, uint8_t config); //dest = RTCC_ALM0/RTCC_ALM1
  void disableAlarm(uint8_t dest); //dest = RTCC_ALM0/RTCC_ALM1
  void alarmOff(uint8_t dest); //dest = RTCC_ALM0/RTCC_ALM1
  unsigned int checkFlag(uint8_t src); //Checks the alarm interrupt flag; src == RTCC_ALM0/RTCC_ALM1
  
  
  //backup battery functions
  void enableVbat(void); //Enables the backup battery
  void disableVbat(void); //Disables the backup battery
  
  
  //get functions
  uint8_t getSec (uint8_t src);    //src = RTCC_ALM0/RTCC_ALM1/RTCC_RTCC
  uint8_t getMin (uint8_t src);    //src = RTCC_ALM0/RTCC_ALM1/RTCC_RTCC/RTCC_PWRD/RTCC_PWRU
  uint8_t getHour (uint8_t src);   //src = RTCC_ALM0/RTCC_ALM1/RTCC_RTCC/RTCC_PWRD/RTCC_PWRU
  uint8_t getAmPm (uint8_t src);   //src = RTCC_ALM0/RTCC_ALM1/RTCC_RTCC/RTCC_PWRD/RTCC_PWRU
  uint8_t getDay (uint8_t src);    //src = RTCC_ALM0/RTCC_ALM1/RTCC_RTCC/RTCC_PWRD/RTCC_PWRU
  uint8_t getDate (uint8_t src);   //src = RTCC_ALM0/RTCC_ALM1/RTCC_RTCC/RTCC_PWRD/RTCC_PWRU
  uint8_t getMonth (uint8_t src);  //src = RTCC_ALM0/RTCC_ALM1/RTCC_RTCC/RTCC_PWRD/RTCC_PWRU
  uint8_t getYear ();              //src = RTCC_RTCC

  //set functions
  //dest = RTCC_RTCC/RTCC_ALM0/RTCC_ALM1
  void setSec(uint8_t dest, uint8_t value); 
  void setMin(uint8_t dest, uint8_t value); 
  void setHour(uint8_t dest, uint8_t value, uint8_t ampm); 
  void setHour(uint8_t dest, uint8_t value); 
  void setDay(uint8_t dest, uint8_t value);
  void setDate(uint8_t dest, uint8_t value);
  void setMonth(uint8_t dest, uint8_t value);
  //dest = RTCC_RTCC
  void setYear(uint8_t value);
  
  
};



#endif




