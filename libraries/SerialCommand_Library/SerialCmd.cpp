/**
 * SerialCmd.cpp - Class for serial commands between arduinos
 *
 * Author: Tallis Dawson
 * Date: September 17, 2019
 **/
 
 
#include "SerialCmd.h"

SerialCmd::SerialCmd(HardwareSerial& serial, Adafruit_LiquidCrystal& lcd) : _serial(serial), _lcd(lcd)
{
	_serial.begin(9600);
}

int SerialCmd::readMsg()
{	
	_lcd.setCursor(0,0);
	int msg = _serial.read();
	
	if(msg > 0){
	char msgPrint[17];
	sprintf(msgPrint, "rec'd msg: %d", msg);
	_lcd.print(msgPrint);
	
	return msg;
	}
	else {
	_lcd.print("     No Msg     ");
	return 0;
	}
}

void SerialCmd::sendSense(bool sense){
	sendMsg((sense) ? 103 : 104);
}

bool SerialCmd::checkForMsg() {
	if(readMsg()== 101) {
		//Send ready and wait for reply
		_serial.write(102);
		_lcd.setCursor(0,1);
		_lcd.print("    Sent 102    ");
		
		int i = 0;
		while (_serial.available() <= 0 && i < 300){
		  ++i;
		  delay(1);
	}
	return (_serial.available() > 0);
	}
	else { return false;}
}

bool SerialCmd::sendMsg(int msg)
{
	_serial.write(101);

	_lcd.setCursor(0,1);
	_lcd.print("    Sent 101    ");
	
	//wait for reply
	int i = 0;
	while (!_serial.available() && i < 300){
		++i;
		delay(1);
	}
	if(readMsg() == 102) {
		_serial.write(msg);
		char msgPrint[17];
		sprintf(msgPrint, "sent msg: %d ", msg);
		_lcd.setCursor(0,1);
		_lcd.print(msgPrint);
	return true;
	}
	else{
		_lcd.setCursor(0,1);
		_lcd.print("  Send Failed   ");
		return false;
	}
}
