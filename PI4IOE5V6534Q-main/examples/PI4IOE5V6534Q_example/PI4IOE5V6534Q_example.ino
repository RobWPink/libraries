#include <Arduino.h>
#include <PI4IOE5V6534Q.h>

// Power Supply Enable Pin
#define TRACO_24VDC 23

// Test pin
#define TEST_PIN 13

#define IO_ADDR 0x23

TwoWire i2c;

PI4IOE5V6534Q io(IO_ADDR, i2c);

uint8_t ecu_i2c_addr = 0x00;

void setup() {
  // put your setup code here, to run once:


  // Turn on Power Supply
  pinMode(TRACO_24VDC, OUTPUT);
  digitalWrite(TRACO_24VDC, HIGH);

  // Init TEST_PIN
  pinMode(TEST_PIN, OUTPUT);
  digitalWrite(TEST_PIN, HIGH);

  // Initialize serial
  Serial.begin(115200);  // debug output

  io.begin();
  if (!io.isConnected()) {
    Serial.println("IO Expander not connected!");
  } else
    Serial.println("IO Expander present.");
  delay(1000);

  Serial.println("Configure Pins."); 
  io.pinMode(P0_0, OUTPUT);
  io.pinMode(P0_1, INPUT_PULLDOWN);
  io.pinMode(P0_2, INPUT_PULLDOWN, true);
  io.pinMode(P0_3, INPUT);
  io.pinMode(P0_4, INPUT, true);
  io.pinMode(P0_5, INPUT_PULLUP);
  io.pinMode(P0_6, INPUT_PULLUP, true);
  io.pinMode(P0_7, OUTPUT);
  
  io.pinMode(P1_0, INPUT_PULLUP);
  io.pinMode(P1_1, INPUT_PULLUP);
  io.pinMode(P1_2, INPUT_PULLUP);
  io.pinMode(P1_3, INPUT_PULLUP, true);
  io.pinMode(P1_4, INPUT_PULLUP, true);
  io.pinMode(P1_5, INPUT_PULLUP, true);
  io.pinMode(P1_6, OUTPUT);
  io.pinMode(P1_7, OUTPUT);

  io.pinMode(P2_0, INPUT_PULLDOWN, true);
  io.pinMode(P2_1, INPUT_PULLDOWN, true);
  io.pinMode(P2_2, INPUT_PULLDOWN, true);
  io.pinMode(P2_3, INPUT_PULLDOWN, true);
  io.pinMode(P2_4, INPUT_PULLDOWN);
  io.pinMode(P2_5, INPUT_PULLDOWN); 
  io.pinMode(P2_6, INPUT_PULLDOWN);
  io.pinMode(P2_7, INPUT_PULLDOWN);

  if(!io.portMode(IO_EXP_PORT3, INPUT_PULLUP))
  {
    Serial.println("P3 error.");
  }
  

  io.pinMode(P4_0, OUTPUT);
  io.pinMode(P4_1, OUTPUT);

  Serial.println("Set Output initial State");  
  io.digitalWrite(P0_0, HIGH);
  io.digitalWrite(P0_7, HIGH);

  io.digitalWrite(P1_6, HIGH);
  io.digitalWrite(P1_7, HIGH);

  io.digitalWrite(P4_0, HIGH);
  io.digitalWrite(P4_1, HIGH);
  
  Serial.println("\n\rEnd Setup.");

  delay(100);
}

void loop() {
  // put your main code here, to run repeatedly:

  static uint8_t pin_val = LOW;
  // uint8_t input_pin_val = 0;
  // uint8_t input_pin_val0 = 0;
  uint8_t input_pin_val1 = 0;
  uint8_t input_pin_val2 = 0;
  uint8_t input_pin_val3 = 0;
  uint8_t input_pin_val4 = 0;
  uint8_t input_pin_val5 = 0;
  uint8_t input_pin_val6 = 0;
  // uint8_t input_pin_val7 = 0;
  uint8_t input_pin_val8 = 0;
  uint8_t input_pin_val9 = 0;
  uint8_t input_pin_val10 = 0;
  uint8_t input_pin_val11 = 0;
  uint8_t input_pin_val12 = 0;
  uint8_t input_pin_val13 = 0;
  // uint8_t input_pin_val14 = 0;
  // uint8_t input_pin_val15 = 0;
  uint8_t input_pin_val16 = 0;
  uint8_t input_pin_val17 = 0;
  uint8_t input_pin_val18 = 0;
  uint8_t input_pin_val19 = 0;
  uint8_t input_pin_val20 = 0;
  uint8_t input_pin_val21 = 0;
  uint8_t input_pin_val22 = 0;
  uint8_t input_pin_val23 = 0;

  static uint32_t counter = 1;

  Serial.print("Begin Loop: ");
  Serial.println(counter);

  io.readPort(IO_EXP_PORT3, &ecu_i2c_addr);
  Serial.print("I2C Address: 0x");
  Serial.println(ecu_i2c_addr, HEX);

  // io.digitalRead(P0_0, &input_pin_val0);
  // Serial.print("P0_0: ");
  // Serial.println(input_pin_val0, BIN);

  io.digitalRead(P0_1, &input_pin_val1);
  Serial.print("P0_1: ");
  Serial.println(input_pin_val1, BIN);

  io.digitalRead(P0_2, &input_pin_val2);
  Serial.print("P0_2: ");
  Serial.println(input_pin_val2, BIN);

  io.digitalRead(P0_3, &input_pin_val3);
  Serial.print("P0_3: ");
  Serial.println(input_pin_val3, BIN);

  io.digitalRead(P0_4, &input_pin_val4);
  Serial.print("P0_4: ");
  Serial.println(input_pin_val4, BIN);

  io.digitalRead(P0_5, &input_pin_val5);
  Serial.print("P0_5: ");
  Serial.println(input_pin_val5, BIN);

  io.digitalRead(P0_6, &input_pin_val6);
  Serial.print("P0_6: ");
  Serial.println(input_pin_val6, BIN);
  
  // io.digitalRead(P0_7, &input_pin_val7);
  // Serial.print("P0_7: ");
  // Serial.println(input_pin_val7, BIN);
  Serial.println();

  io.digitalRead(P1_0, &input_pin_val8);
  Serial.print("P1_0: ");
  Serial.println(input_pin_val8, BIN);

  io.digitalRead(P1_1, &input_pin_val9);
  Serial.print("P1_1: ");
  Serial.println(input_pin_val9, BIN);

  io.digitalRead(P1_2, &input_pin_val10);
  Serial.print("P1_2: ");
  Serial.println(input_pin_val10, BIN);

  io.digitalRead(P1_3, &input_pin_val11);
  Serial.print("P1_3: ");
  Serial.println(input_pin_val11, BIN);

  io.digitalRead(P1_4, &input_pin_val12);
  Serial.print("P1_4: ");
  Serial.println(input_pin_val12, BIN);

  io.digitalRead(P1_5, &input_pin_val13);
  Serial.print("P1_5: ");
  Serial.println(input_pin_val13, BIN);

  // io.digitalRead(P1_6, &input_pin_val14);
  // Serial.print("P1_6: ");
  // Serial.println(input_pin_val14, BIN);

  // io.digitalRead(P1_7, &input_pin_val15);
  // Serial.print("P1_7: ");
  // Serial.println(input_pin_val15, BIN);
  Serial.println();
  
  io.digitalRead(P2_0, &input_pin_val16);
  Serial.print("P2_0: ");
  Serial.println(input_pin_val16, BIN);

  io.digitalRead(P2_1, &input_pin_val17);
  Serial.print("P2_1: ");
  Serial.println(input_pin_val17, BIN);

  io.digitalRead(P2_2, &input_pin_val18);
  Serial.print("P2_2: ");
  Serial.println(input_pin_val18, BIN);

  io.digitalRead(P2_3, &input_pin_val19);
  Serial.print("P2_3: ");
  Serial.println(input_pin_val19, BIN);

  io.digitalRead(P2_4, &input_pin_val20);
  Serial.print("P2_4: ");
  Serial.println(input_pin_val20, BIN);

  io.digitalRead(P2_5, &input_pin_val21);
  Serial.print("P0_5: ");
  Serial.println(input_pin_val21, BIN);

  io.digitalRead(P2_6, &input_pin_val22);
  Serial.print("P2_6: ");
  Serial.println(input_pin_val22, BIN);

  io.digitalRead(P2_7, &input_pin_val23);
  Serial.print("P2_7: ");
  Serial.println(input_pin_val23, BIN);


  //pin_val = (pin_val == LOW) ? HIGH : LOW;

  digitalWrite(TEST_PIN, pin_val);  //Reference signal from mega

  Serial.print("End Loop: ");
  Serial.println(counter++);
  Serial.println();
  delay(1000);
}
