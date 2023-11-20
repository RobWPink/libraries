/**
 * LedHelper.cpp - Class for led functions
 *
 * Author: Tallis Dawson
 * Edited: Robert Pink January 11, 2023
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
	if(brightness < 1) {
		matrix.clear();
		matrix.fillRect(0,0, 8,8, LED_OFF);
		matrix.setBrightness(brightness);
		matrix.writeDisplay();
	}
	else{
	
		matrix.clear();
		matrix.fillRect(0,0, 8,8, LED_ON);
		matrix.setBrightness(brightness);
		matrix.writeDisplay();
	}
}

void SmallMatrix::displayChar(char c, bool invert)
{
  uint16_t inv = (uint16_t)invert;
  matrix.setRotation(0);
	matrix.clear();
 if(invert){matrix.fillRect(0,0, 8,8, LED_ON);}
  switch(c){
  case 'L':
    matrix.drawLine(0,4,7,4,!inv);
    matrix.drawLine(7,8,7,1,!inv);
    break;
  case 'M':
    matrix.drawLine(0,0,7,0,!inv);
    matrix.drawLine(0,0,7,3,!inv);
    matrix.drawLine(7,3,0,6,!inv);
    matrix.drawLine(0,6,7,6,!inv);
    break;
  case 'H':
    matrix.drawLine(0,1,7,1,!inv);
    matrix.drawLine(4,1,4,4,!inv);
    matrix.drawLine(0,4,7,4,!inv);
    break;
  case '1':
    matrix.drawLine(7,1,7,5,!inv);
    matrix.drawLine(7,3,0,3,!inv);
    matrix.drawLine(0,3,2,5,!inv);
    break;
  case '2':
    matrix.drawLine(7,1,7,5,!inv);
    matrix.drawLine(7,5,4,5,!inv);
    matrix.drawLine(4,5,4,1,!inv);
    matrix.drawLine(4,1,1,1,!inv);
    matrix.drawLine(1,1,1,5,!inv);
    break;
  case '3':
    matrix.drawLine(7,1,7,5,!inv);
    matrix.drawLine(7,1,1,1,!inv);
    matrix.drawLine(1,1,1,5,!inv);
    matrix.drawLine(4,1,4,5,!inv);
    break;
  case '4':
    matrix.drawLine(7,1,1,1,!inv);
    matrix.drawLine(4,1,4,4,!inv);
    matrix.drawLine(4,4,1,4,!inv);
    break;
  case '5':
    matrix.drawLine(7,5,7,1,!inv);
    matrix.drawLine(7,1,4,1,!inv);
    matrix.drawLine(4,1,4,5,!inv);
    matrix.drawLine(4,5,1,5,!inv);
    matrix.drawLine(1,5,1,1,!inv);
    break;
  case '6':
    matrix.drawLine(7,5,7,1,!inv);
    matrix.drawLine(7,1,4,1,!inv);
    matrix.drawLine(4,1,4,5,!inv);
    matrix.drawLine(4,5,1,5,!inv);
    matrix.drawLine(1,5,1,1,!inv);
    matrix.drawLine(7,5,4,5,!inv);
    break;
  case '7':
    matrix.drawLine(1,5,1,1,!inv);
    matrix.drawLine(1,1,7,1,!inv);
    break;
  case '8':
    matrix.drawLine(7,5,7,1,!inv);
    matrix.drawLine(7,1,4,1,!inv);
    matrix.drawLine(4,1,4,5,!inv);
    matrix.drawLine(4,5,1,5,!inv);
    matrix.drawLine(1,5,1,1,!inv);
    matrix.drawLine(7,5,4,5,!inv);
    matrix.drawLine(1,1,4,1,!inv);
    break;
  case '9':
    matrix.drawLine(7,5,7,1,!inv);
    matrix.drawLine(7,1,4,1,!inv);
    matrix.drawLine(4,1,4,5,!inv);
    matrix.drawLine(4,5,1,5,!inv);
    matrix.drawLine(1,5,1,1,!inv);
    matrix.drawLine(1,1,4,1,!inv);
    break;
/*  case "10":
    matrix.drawLine(1,1,1,7,!inv);
    matrix.drawLine(1,7,7,4,!inv);
    matrix.drawLine(7,4,1,4,!inv);
    matrix.drawLine(1,4,1,1,!inv);
    matrix.drawLine(1,6,7,6,!inv);
    break;
  case "11":
    matrix.drawLine(1,5,7,5,!inv);
    matrix.drawLine(1,3,7,3,!inv);

    break;
  case "12":
    matrix.drawLine(7,5,7,1,!inv);
    matrix.drawLine(7,1,4,1,!inv);
    matrix.drawLine(4,1,4,5,!inv);
    matrix.drawLine(4,5,1,5,!inv);
    matrix.drawLine(1,5,1,1,!inv);
    matrix.drawLine(1,1,4,1,!inv);
    break;
  case "13":
    matrix.drawLine(7,5,7,1,!inv);
    matrix.drawLine(7,1,4,1,!inv);
    matrix.drawLine(4,1,4,5,!inv);
    matrix.drawLine(4,5,1,5,!inv);
    matrix.drawLine(1,5,1,1,!inv);
    matrix.drawLine(1,1,4,1,!inv);
    break;
  case "14":
    matrix.drawLine(7,5,7,1,!inv);
    matrix.drawLine(7,1,4,1,!inv);
    matrix.drawLine(4,1,4,5,!inv);
    matrix.drawLine(4,5,1,5,!inv);
    matrix.drawLine(1,5,1,1,!inv);
    matrix.drawLine(1,1,4,1,!inv);
    break;
  case "15":
    matrix.drawLine(7,5,7,1,!inv);
    matrix.drawLine(7,1,4,1,!inv);
    matrix.drawLine(4,1,4,5,!inv);
    matrix.drawLine(4,5,1,5,!inv);
    matrix.drawLine(1,5,1,1,!inv);
    matrix.drawLine(1,1,4,1,!inv);
    break;*/
  default:
    break;
  }
  matrix.setBrightness(15);
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
	if(num > 9999){ num = num/10; matrix.print(num); matrix.writeDigitNum(4,num%10,1); }
	else{matrix.print(num);}
	matrix.writeDisplay();	
}
