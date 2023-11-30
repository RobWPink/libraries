/*******************************************************************************
DebouncedButtons Arduino library http://libahunt.ee/debouncedbuttons
by Anna Jõgi a.k.a Libahunt
Public domain
*******************************************************************************/
#ifndef DEBOUNCEDBUTTONS_H
#define DEBOUNCEDBUTTONS_H

#include "Arduino.h"

class DebouncedButton {

  public:
  
    DebouncedButton(int pinNumber, unsigned long debounceDelay);
    boolean dbRead();
		
	private:

		int pin;
    unsigned long dbDelay;
    boolean lastDbState;
    unsigned long lastDbTime;
    boolean value;

};
#endif
