/**
 * LedHelper.cpp - Class for led functions
 *
 * Author: Tallis Dawson
 * Date: October 03, 2019
 **/
 
 
#include "LedHelper.h"



SmallMatrix::SmallMatrix(uint8_t address) : addr(address)
{
	matrix = Adafruit_8x8matrix();
}

void SmallMatrix::begin()
{
	matrix.begin(addr);
	
	matrix.setRotation(3);
    matrix.clear();
    matrix.setBrightness(15);
    matrix.drawCircle(3,3, 3, LED_ON);
    matrix.writeDisplay();
}

void SmallMatrix::squareDisplay(int brightness)
{
	if(brightness < 1) { brightness = 1; }
	
	matrix.clear();
	matrix.fillRect(0,0, 8,8, LED_ON);
	matrix.setBrightness(brightness);
	matrix.writeDisplay();
}

bool SmallMatrix::posDisplay(int pos)
{
	if(pos < 105 || pos > 112) { return false; }
	
	matrix.clear();
	matrix.drawCircle(3,3, 3, LED_ON);

	switch (pos) {
	case 105 :
	  matrix.drawLine(0,3,6,3,LED_ON);
	  matrix.drawPixel(6,3,LED_OFF);
	  break;
	case 106 :
	  matrix.drawLine(5,5,1,1,LED_ON);
	  matrix.drawPixel(5,5,LED_OFF);
	  break;
	case 107 :
	  matrix.drawLine(3,6,3,0,LED_ON);
	  matrix.drawPixel(3,6,LED_OFF);
	  break;
	case 108 :
	  matrix.drawLine(1,5,5,1,LED_ON);
	  matrix.drawPixel(1,5,LED_OFF);
	  break;
	case 109 :
	  matrix.drawLine(0,3,6,3,LED_ON);
	  matrix.drawPixel(0,3,LED_OFF);    
	  break;
	case 110 :
	  matrix.drawLine(5,5,1,1,LED_ON);
	  matrix.drawPixel(1,1,LED_OFF);
	  break;
	case 111 :
	  matrix.drawLine(3,6,3,0,LED_ON);
	  matrix.drawPixel(3,0,LED_OFF);
	  break;
	case 112 :
	  matrix.drawLine(1,5,5,1,LED_ON);
	  matrix.drawPixel(5,1,LED_OFF);
	  break;
	}
	matrix.writeDisplay();
	return true;
}


LargeMatrix::LargeMatrix(uint8_t address) : addr(address)
{
	matrix = Adafruit_7segment();
}

void LargeMatrix::begin()
{
	matrix.begin(addr);
	
	matrix.clear();
    matrix.writeDisplay();
}

void LargeMatrix::writeInt(int num)
{
	matrix.clear();
	matrix.print(num);
	matrix.writeDisplay();	
}

