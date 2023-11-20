/**
 * @file MCP47CXBXX
 * @brief Test Code to excercise the MCP47CXBXX on the ECU
 * @author Christopher Long, ONEH2
 * @date July, 13 2022
 */
#include <Arduino.h>
#include <MCP47CXBXX.h>

// Power Supply Enable Pin
#define TRACO_24VDC 23

#define DAC_ADDR 0x60


MCP47CXBXX dac(DAC_ADDR, &Wire);

void setup() {
  // put your setup code here, to run once:

  // Turn on Power Supply
  pinMode(TRACO_24VDC, OUTPUT);
  digitalWrite(TRACO_24VDC, HIGH);

  // Initialize serial
  Serial.begin(57600); // debug output

  dac.begin();
  if (!dac.isConnected())
  {
    Serial.println("DAC not connected!");
  }
  else Serial.println("DAC present.");
  
}

void loop() {
  // put your main code here, to run repeatedly:
  static uint16_t dac_out = 0;

  if (dac.wr_VOUT_0(dac_out++) != DAC_RW_SUCCESS)
  {
    Serial.println("DAC Vout0 write failed!");
  }  
  else Serial.println("DAC Vout0 write success.");

  delay(100);
}
