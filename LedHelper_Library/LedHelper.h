/**
 * LedHelper.h - Class for led functions
 *
 * Author: Tallis Dawson
 * Date: October 03, 2019
 **/
 
#ifndef LEDHELPER_H
#define LEDHELPER_H

#include "Arduino.h"

#include <Wire.h> //For I2C comm
#include "Adafruit_LEDBackpack.h"
#include "Adafruit_LiquidCrystal.h"
#include <Adafruit_GFX.h>

class SmallMatrix
{
	public:
		SmallMatrix(uint8_t address);
		
		void begin();
		
		bool posDisplay(int pos);
		void squareDisplay(int brightness);
		void displayPlay(bool invert);
		void displayHollowPlay(bool invert);
		void displayPause(bool invert);
		void displayStop(bool invert);
		void displayQuadrants(bool a, bool b, bool c, bool d, bool p1, bool p2);
		void halfMatrix(bool a, bool b);
		void WaterDropDisplay(bool invert);
    	void displayChar(char c, bool invert);
		
		
		~SmallMatrix() {};
		
	private:
		uint8_t addr;
		Adafruit_8x8matrix matrix;
};

class LargeMatrix
{
	public:
		LargeMatrix(uint8_t address);
		
		void writeInt(int num);
		
		void begin();
		
		~LargeMatrix() {};
		
	private:
		uint8_t addr;
		Adafruit_7segment matrix;
};

#endif 
