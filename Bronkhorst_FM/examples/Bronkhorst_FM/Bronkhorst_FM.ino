/**
 * @file Bronkhorst Flow Meter
 * @brief Program to communicate with a Bronkhorst Flow Meter
 * @author Christopher Long, ONEH2
 * @date May, 18 2022
 * @date Apr, 28 2023
 
 *last modified 06/24 by Sandeep
 */

#include <Arduino.h>
#include "bronkhorstFM.h"

// Power Supply Enable Pin
#define TRACO_24VDC 23

Bronkhorst_FM flow_meter(Serial3);
int count = 0;
int measure = 0;
float fmeasure = 0;
float temp = 0;
float actDensity = 0;
long valOutput = 0;
int setpoint = 16000;
int target_setpoint = 0x7D00;
int current_mode = 0;


void setup()
{
  // Turn on Power Supply
  pinMode(TRACO_24VDC, OUTPUT);
  digitalWrite(TRACO_24VDC, HIGH);
  
  // Start Serial ports
  Serial.begin(9600); // to terminal
  Serial3.begin(38400, SERIAL_8N1); // to flow meter

  while (current_mode != RS232)
  {
    flow_meter.wr_controlmode(RS232);
    delay(10);
    current_mode = flow_meter.rd_controlmode();
    Serial.print("Starting Mode: ");
    Serial.println(current_mode, HEX);
    delay(300);
  }

  Serial.println("End Setup.");
}

void loop()
{
  Serial.print(count++);
  Serial.println(": "); 
  
  if (current_mode == RS232)
  {
    //flow_meter.wr_flowunit_kg_h();
    // Serial.print("sensor ");
    // Serial.print(flow_meter.rd_sensor());
    Serial3.print(":058001010E32\n\r");

    flow_meter.wr_setpoint(target_setpoint);
    setpoint = flow_meter.rd_setpoint();
    Serial.print(" Setpoint: ");
    Serial.print(setpoint);
    delay(20);

    temp = flow_meter.rd_temp();
    Serial.print(" Temp: ");
    Serial.print(temp);
    delay(20);

    measure = flow_meter.rd_measure();
    Serial.print(" Measure: ");
    Serial.print(measure);
    delay(20);
  
    fmeasure = flow_meter.rd_fmeasure();
    Serial.print(" FMeasure: ");
    Serial.print(fmeasure);
    delay(20);

    valOutput = flow_meter.rd_valOutput();
    double percentage;
    long max_Val = 16773519;
    percentage = (float)valOutput / max_Val * 100;
    Serial.print(" valveOutputDecimal, valveOutputPercent : ");
    Serial.print(valOutput);Serial.print(", ");Serial.print(percentage);
    delay(20);

    actDensity = flow_meter.rd_actDensity();
    Serial.print(" ActualDensity: ");
    Serial.print(actDensity);
    delay(20);

    

  }
    
  Serial.println();
  delay(500);  

}