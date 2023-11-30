/**
 * StepMotor.cpp - Library for controlling hydrogen actuators
 *
 * Author: Tallis Dawson
 * Date: October 29th, 2019
 **/
 
#include "Arduino.h"
#include "MQTTHelper.h"
 
MQTTHelper::MQTTHelper()
{
	Adafruit_MQTT_FONA temp_mqtt(&fona, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);
	*mqtt = temp_mqtt;
	
	
	feed_indicator = Adafruit_MQTT_Publish(temp_mqtt, AIO_USERNAME "/feeds/light");
	feed_input = Adafruit_MQTT_Subscribe(temp_mqtt, AIO_USERNAME "/feeds/input");
	
	pinMode(FONA_RST, OUTPUT);
	digitalWrite(FONA_RST, HIGH);
	pinMode(FONA_PWRKEY, OUTPUT);

	//power on
	digitalWrite(FONA_PWRKEY, LOW);
	delay(100);
	digitalWrite(FONA_PWRKEY, HIGH);

	//set up module
	fonaSS.begin(115200);
	fonaSS.println("AT+IPR=9600");//configure baud rate
	fonaSS.begin(9600);
	
	if (! fona.begin(fonaSS)) {
		while(1); // Don't proceed if it couldn't find the device
	}
	
	uint8_t imeiLen = fona.getIMEI(imei);
	if (imeiLen <= 0) {
		while(1); // Don't proceed if it couldn't find the imei
	}
	
	fona.setFunctionality(1); // AT+CFUN=1
	fona.setNetworkSettings(F("hologram")); // For Hologram SIM card
	
	while (!fona.enableGPS(true)) {
		delay(300); // Retry every .3s
	}

	// Disable GPRS just to make sure it was actually off so that we can turn it on
	fona.enableGPRS(false);

	// Turn on GPRS
	while (!fona.enableGPRS(true)) {
	delay(300); // Retry every .3s
	}


}
	