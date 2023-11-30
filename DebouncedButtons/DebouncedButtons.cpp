/*******************************************************************************
DebouncedButtons Arduino library http://libahunt.ee/debouncedbuttons
by Anna Jõgi a.k.a Libahunt
Public domain
*******************************************************************************/

#include "Arduino.h"
#include "DebouncedButtons.h"


DebouncedButton::DebouncedButton(int pinNumber, unsigned long debounceDelay) {

  pin = pinNumber;
  dbDelay = debounceDelay;
  lastDbState = digitalRead(pin);
  value = lastDbState;
  lastDbTime = millis();
	
}

boolean DebouncedButton::dbRead() {
  boolean dbState = digitalRead(pin);
  if (dbState != lastDbState) {// if reading changes
    lastDbTime = millis();     //      we reset timer
  }
  lastDbState = dbState;       //save current reading for next loop
  if ((millis() - lastDbTime) > dbDelay) {  //if reading has been constant for long enough
    value = dbState;                        //    we assign reading as new value
  }
  return value; //return value
}
