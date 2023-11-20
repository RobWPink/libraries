#include <ADS7828.h>

#include <Arduino.h>
#include <PI4IOE5V6534Q.h>
#define IO_ADDR 0x23
PI4IOE5V6534Q io(IO_ADDR, &Wire);
uint8_t ecu_i2c_addr = 0x00;

//-----------Power Enable Digital Outputs-----------//
#define ESTOP_BREAK 40
#define LED_PWR 22
#define TRACO_24VDC 23
 
  uint8_t input_pin_val = 0;
  uint8_t input_pin_val1 = 0;
  uint8_t input_pin_val2 = 0;
  uint8_t input_pin_val3 = 0;
  uint8_t input_pin_val4 = 0;
  uint8_t input_pin_val5 = 0;

  uint8_t input_pin_val6 = 0;

void setup() {
   Serial.begin(9600);
  io.begin();
    if (!io.isConnected())
  {
    Serial.println("IO Expander not connected!");
  }
  else
    Serial.println("IO Expander present.");   
  delay(1000);
  //io.pinMode(P0_0, OUTPUT);
  //io.pinMode(P0_7, OUTPUT);
  //io.pinMode(P1_0, OUTPUT);

  delay(500);

  io.portMode(IO_EXP_PORT3, INPUT);
  io.pinMode(P0_1, INPUT_PULLUP);
  io.pinMode(P0_2, INPUT_PULLUP);
  io.pinMode(P0_3, INPUT_PULLUP);
  io.pinMode(P0_4, INPUT_PULLUP);
  io.pinMode(P0_5, INPUT_PULLUP);
  io.pinMode(P1_0, INPUT_PULLUP);
  io.pinMode(P1_1, INPUT_PULLUP);
  io.pinMode(P1_2, INPUT_PULLUP);
  io.pinMode(P1_3, INPUT_PULLUP);
  io.pinMode(P1_4, INPUT_PULLUP);
  io.pinMode(P1_5, INPUT_PULLUP);
  io.pinMode(P1_6, INPUT_PULLUP);



  io.pinMode(P0_0, INPUT);
  io.readPort(IO_EXP_PORT3, &ecu_i2c_addr);
  // put your setup code here, to run once:
}

void loop() {
  // put your main code here, to run repeatedly:
    io.digitalRead(P1_1, &input_pin_val);
  Serial.print("P1_1: ");    
  Serial.println(input_pin_val, BIN);
    io.digitalRead(P1_2, &input_pin_val1);
  Serial.print("P1_2: ");    
  Serial.println(input_pin_val1, BIN);
    io.digitalRead(P1_3, &input_pin_val2);
  Serial.print("P1_3: ");    
  Serial.println(input_pin_val2, BIN);
    io.digitalRead(P1_4, &input_pin_val3);
  Serial.print("P1_4: ");    
  Serial.println(input_pin_val3, BIN);
}
