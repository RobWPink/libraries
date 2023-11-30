/**
 * SerialCmd.h - Class for serial commands between arduinos
 *
 * Author: Tallis Dawson
 * Date: September 17, 2019
 **/
 
#ifndef SerialCmd_h
#define SerialCmd_h

#include "Arduino.h"

#include <Wire.h> //For I2C comm
#include "Adafruit_LiquidCrystal.h"
#include <Adafruit_GFX.h>

class SerialCmd
{
	public:
		SerialCmd(HardwareSerial& serial, Adafruit_LiquidCrystal& lcd);
		
		bool checkForMsg();
		int readMsg();
		
		void sendSense(bool sense);
		
		bool sendMsg(int msg);
		
		~SerialCmd() {};
		
	private:
		HardwareSerial& _serial;
		Adafruit_LiquidCrystal& _lcd;
};

#endif 
 