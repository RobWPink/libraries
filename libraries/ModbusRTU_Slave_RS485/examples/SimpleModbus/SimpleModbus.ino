#include <ModbusRTUSlave.h>
#include <Wire.h> //For I2C comm
#include <Adafruit_GFX.h>
#include <LedHelper.h>
#include "Adafruit_LEDBackpack.h"
#include "Adafruit_LiquidCrystal.h"

u16 _D[100];
u8 _M[50];

const int myPin = 4;

ModbusRTUSlave rtu(1, &Serial);
LargeMatrix bigMatrix[3] = {LargeMatrix(0x73), LargeMatrix(0x74), LargeMatrix(0x75) };
const int LED_PWR = 22;

void setup() {
  pinMode(myPin, OUTPUT);
  pinMode(LED_PWR, OUTPUT);
  digitalWrite(LED_PWR, HIGH);
  matrixSetup();
 
  bigMatrix[0].writeInt(200);
 
  rtu.addWordArea(40000, _D, 100);
  rtu.addBitArea(0x1000, _M, 50);
  rtu.begin(9600);
}

void loop() {
  rtu.process();
  digitalWrite(myPin, _D[0] > 0 ? HIGH:LOW);
  if(digitalRead(myPin) == HIGH){
    bigMatrix[0].writeInt(1);
  } else {
    bigMatrix[0].writeInt(0);
  }
}

void matrixSetup() {
  //begin matrices
  for(int i = 0; i < 3; i++){
    bigMatrix[i].begin();
  }

}
