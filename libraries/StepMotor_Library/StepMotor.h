/**
 * StepMotor.h - Library for controlling hydrogen actuators
 *
 * Author: Tallis Dawson
 * Date: August 30th, 2019
 **/
 
#ifndef StepMotor_h
#define StepMotor_h

#include "Arduino.h"

class StepMotor
{
	public:
		StepMotor(int DIRECTION_PIN, int RESET_PIN, int CTRL_PIN,
		int VREF_PIN, int HALF_FULL_PIN, int ENABLE_PIN,
		int CLOCK_PIN, int SENSE_PIN, int vRefValue = 20);
		
		void step(int noOfSteps, bool clockwise);
		void changeTorque(int vRefValue);
		
		bool moveToPos(int pos);
		
		int getCurrentSense() const;
		
		~StepMotor() {};
		
	private:
		const int _DIRECTION,  _RESET, _CTRL, _VREF, _HALF_FULL,
			_ENABLE, _CLOCK, _SENSE;
		
		int vRef, currentSense;
		
		static const int positions[];
		
		int potValToSteps(int potVal);
		

};

#endif 