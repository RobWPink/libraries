/**
 * MQTTHelper.h
 *
 * Author: Tallis Dawson
 * Date: October 28, 2019
 **/
 
#ifndef MQTTHelper_H
#define MQTTHelper_H

#include "Arduino.h"

#include "Adafruit_FONA.h"
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_FONA.h"

#include <SoftwareSerial.h>


#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883
#define AIO_USERNAME    "OneH2"
#define AIO_KEY         "f810696709b04f9ab7bb3cb34bd15bcd"

class MQTTHelper
{
	public:
		MQTTHelper();
		
		//void begin();
		
		~MQTTHelper() {};
		
	private:
		const int FONA_PWRKEY = 6;
		const int FONA_RST = 7;
		const int FONA_TX = 10; // Microcontroller RX
		const int FONA_RX = 11; // Microcontroller TX
		
		char imei[16] = {0};
		
		Adafruit_MQTT_Publish feed_indicator;
		Adafruit_MQTT_Subscribe feed_input;
		
		SoftwareSerial fonaSS = SoftwareSerial(FONA_TX, FONA_RX);
		SoftwareSerial *fonaSerial = &fonaSS;
		
		Adafruit_FONA_LTE fona = Adafruit_FONA_LTE();
		Adafruit_MQTT_FONA *mqtt;
};

#endif 