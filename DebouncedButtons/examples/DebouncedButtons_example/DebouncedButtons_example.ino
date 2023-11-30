/*******************************************************************************
DebouncedButtons Arduino library http://libahunt.ee/debouncedbuttons
by Anna Jõgi a.k.a Libahunt
Public domain
*******************************************************************************/

#include <DebouncedButtons.h>

//CREATE INSTANCES OF DebouncedButton CLASS for each button
//DebouncedButton myButtonObjectName = DebouncedButton(int pinNumber, unsigned long debounceDelay);
  
  DebouncedButton button1 = DebouncedButton(3,50); //Button attached to pin 3. 
                                                   //It has debounce delay 50 milliseconds.
                                                   
  DebouncedButton button2 = DebouncedButton(4,2000); //Button attached to pin 4. 
                                                     //This has debounce delay 2000 ms = 2 seconds.

//Debounce delay 2 seconds is very long, but lets you observe the meaning of debouncing.
//The press or releasing of the button registers after the state has been stable for 2 seconds.
//If you keep toggling the button during that time, it does not register at all.
//50 ms is a good starting value if you don't know what to put in debounceDelay.



void setup() {
  
  /*Serial is for example purposes*/
  Serial.begin(9600);
  Serial.println("Button on pin 3          Button on pin 4");
  
}

void loop() {
  
  //USAGE
  //the dbRead function has to be called from inside loop
  //and so often that reading is repeated at least few times before debounceDelay time passes
  
  //myButtonObjectName.dbRead() returns boolean value (1 or 0) of the last state seen stable
  //for the timespan of debounceDelay set for that specific buttonObject
  
  //this statement is a debounced equivalent of 
  //boolean myButtonReading = digitalRead(myButtonPinNumber);
  
  boolean button1debounced = button1.dbRead(); 
  boolean button2debounced = button2.dbRead();
  
  
  
  /*Serial is for example purposes*/
  Serial.print("           ");
  Serial.print(button1debounced);  
  Serial.print("                       ");
  Serial.println(button2debounced);
  
  
}







