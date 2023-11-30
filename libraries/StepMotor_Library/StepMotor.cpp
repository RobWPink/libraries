/**
 * StepMotor.cpp - Library for controlling hydrogen actuators
 *
 * Author: Tallis Dawson
 * Date: August 30th, 2019
 **/
 
#include "Arduino.h"
#include "StepMotor.h"
 
StepMotor::StepMotor(int DIRECTION_PIN, int RESET_PIN, int CTRL_PIN,
	int VREF_PIN, int HALF_FULL_PIN, int ENABLE_PIN, int CLOCK_PIN, int SENSE_PIN, int vRefValue) : _DIRECTION(DIRECTION_PIN),
	_RESET(RESET_PIN), _CTRL (CTRL_PIN), _VREF(VREF_PIN), _HALF_FULL(HALF_FULL_PIN), _ENABLE(ENABLE_PIN), _CLOCK(CLOCK_PIN), _SENSE(SENSE_PIN)
{
	pinMode(_DIRECTION, OUTPUT);
	digitalWrite(_DIRECTION, HIGH); // Sets the motor direction to counter clockwise
	
	pinMode(_RESET, OUTPUT);
	digitalWrite(_RESET, HIGH); // Reset with an active low pulse, placed high to avoid reset
	
	pinMode(_CTRL, OUTPUT);
	digitalWrite(_CTRL, HIGH); // Defines the type of control used on motor driver
	
	
	vRef = vRefValue;
	pinMode(_VREF, OUTPUT);
	analogWrite(_VREF, vRef); // Controls current allowed through motor windings.
	//Value ~= 25.5*desired current in Amps. Default value 20
	
	pinMode(_HALF_FULL, OUTPUT);
	digitalWrite(_HALF_FULL, LOW); // Sets motor to full stepping
	
	pinMode(_ENABLE, OUTPUT);
	digitalWrite(_ENABLE, LOW); // Set HIGH to enable motor
	
	pinMode(_CLOCK, OUTPUT);
	digitalWrite(_CLOCK, HIGH); // Control signal for motor, setting from low to high will 
	//result in a step following the rising edge
	
	currentSense = analogRead(_SENSE);
	
}

//Potentiometer values corresponding to positions 0-7
int const StepMotor::positions[] = {0, 128, 256, 384, 512, 640, 768, 896};


//Steps the motor noOfSteps times, moving clockwise if clockwise is true or ccw if false
void StepMotor::step(int noOfSteps, bool clockwise)
{
	if(clockwise) { digitalWrite(_DIRECTION, LOW); }
	else { digitalWrite(_DIRECTION, HIGH); }
	
	digitalWrite(_ENABLE, HIGH);
	for(int i = 0; i < noOfSteps; i++) {
		digitalWrite(_CLOCK,LOW);
		delay(5);
		digitalWrite(_CLOCK,HIGH);
		delay(5);
	}
	digitalWrite(_ENABLE, LOW);
	
	currentSense = analogRead(_SENSE);
}


//Moves motor to prespecified position using pos valuse from 0-7 returns true if completed
bool StepMotor::moveToPos(int pos)
{
	//Checking to make sure pos is correct
	if(pos > 7 || pos < 0) { return false; }
	
	//finiding difference between current location and destination
	int distance = currentSense - positions[pos];
	
	//If neccesary, reverse direction for shorter path
	if (distance > 512) { distance -= 1024; }
    else if (distance < -512) { distance += 1024; }
	
	//determine direction
	bool direction;
	if (distance > 0) { direction = false; }
    else { direction = true; }
	
	step(potValToSteps(distance), direction);
	
	currentSense = analogRead(_SENSE);
	return true;
}

//updates the torque
void StepMotor::changeTorque(int vRefValue)
{
	if(vRefValue >= 20 && vRefValue < 70) { vRef = vRefValue; }
}

//returns the motor sensor value
int StepMotor::getCurrentSense() const { return currentSense;}

//Converts a pot value to motor steps ~private~ *(MAKE MORE PRECISE)
int StepMotor::potValToSteps(int potVal)
{
	return abs(potVal) / 5.12;
}