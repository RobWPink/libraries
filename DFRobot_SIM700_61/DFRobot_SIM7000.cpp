#include <DFRobot_SIM7000.h>

DFRobot_SIM7000::DFRobot_SIM7000(Stream *s):DFRobot_SIMcore(s)
{
	_s = s;
}

union floatBytes{
	float asFloat;
	uint8_t asByte[4];
};


bool DFRobot_SIM7000::start(uint8_t powerPin, uint8_t resetPin, uint32_t timeout){
	_resetPin = resetPin;
	_powerPin = powerPin;
	pinMode(powerPin,OUTPUT);						//Initialize Pin for toggling power to the modem
	digitalWrite(powerPin, HIGH);					//Immediately set HIGH to avoid accidental power toggle
	pinMode(resetPin,OUTPUT);						//Initialize Pin for resetting the modem
	digitalWrite(resetPin, HIGH);					//Immediately set HIGH to avoid accidental reset 
	if(!checkForModem()){							//Test if an AT Modem is present
		digitalWrite(powerPin, LOW);				//If not present, try sending power-on signal
		delay(500);									//Power on signal requires minimum 200ms
		digitalWrite(powerPin, HIGH);				//Power on signal return high 
		uint32_t timeoutEnd = millis() + timeout;	//Generate timeout for waiting for modem
		while(millis() < timeoutEnd){				//Wait for modem to respond
			if(checkSendCmd("AT\r\n", "OK")){		
				return true;						//Return true ff modem replies
			}
		}
		return false;								//Return false if timeout runs out
	}else{
		return true;								//Return true if checkForModem returned try
	}
}

bool DFRobot_SIM7000::restart(uint8_t powerPin, uint8_t resetPin, uint32_t timeout){
	pinMode(resetPin,OUTPUT);						//Initialize Pin for resetting the modem
	digitalWrite(resetPin, LOW);					//Immediately set LOW
	delay(500);
	digitalWrite(resetPin, HIGH);					//Immediately set LOW
	return start(powerPin, resetPin, timeout);
}

bool DFRobot_SIM7000::setBaudRate(int rate){		//Toggle baudrate of modem serial port
  if(rate > 115200){								//Ensure rate is not too large
	rate = 115200;									//Cannot go above 115200
  }													//
  char baudChangeCmd [24] = {0};					//Buffer for generating baud rate command
  sprintf(baudChangeCmd, "AT+IPR=%d\r\n", rate);	//Generate Baud Rate Command
  return checkSendCmd(baudChangeCmd,"OK");			//Send Baud Rate Command
}

bool DFRobot_SIM7000::setRiPin(int setting){
	char baudChangeCmd [24] = {0};					//Buffer for generating baud rate command
	sprintf(baudChangeCmd, "AT+CFGRI=%d\r\n", setting);	//Generate Baud Rate Command
	return checkSendCmd(baudChangeCmd,"OK");		//Send Baud Rate Command
}

bool DFRobot_SIM7000::checkForModem(void){
	return(checkSendCmd("AT\r\n", "OK", 5000));
}

bool DFRobot_SIM7000::toggleEcho(bool setting){
	char echoChangeCmd [8] = {0};					//Buffer for generating echo toggle command
	sprintf(echoChangeCmd, "ATE%d\r\n", setting);	//Generate echo toggle command
	return checkSendCmd(echoChangeCmd,"OK");
}

bool DFRobot_SIM7000::toggleErrorReporting(uint8_t setting){
	char errorReportCmd [14] = {0};					//Buffer for generating echo toggle command
	sprintf(errorReportCmd, "AT+CMEE=%d\r\n", setting);	//Generate echo toggle command
	return checkSendCmd(errorReportCmd,"OK");
}


bool DFRobot_SIM7000::checkSIMStatus(void){
    return(checkSendCmd("AT+CPIN?\r\n","READY"));
}

bool DFRobot_SIM7000::waitForIpState(const char* desiredState, uint32_t timeout){
	char ipState[36] = {};
	uint32_t timeoutEnd = millis() + timeout;	//Generate timeout for waiting for modem
	while(millis() < timeoutEnd){				//Wait for modem to respond
		getIpState(ipState);
		if (strstr(ipState, desiredState) != NULL){
			return true;
		}
		delay(1000);								//Wait between retries
	}
	return false;
}

bool DFRobot_SIM7000::getIpState(char* statusBuffer){
	sendCmd("AT+CIPSTATUS\r\n");
	readBuffer(statusBuffer,36);
	return(strstr(statusBuffer, "OK"));
}

bool DFRobot_SIM7000::checkNetStatus(char* respondBuffer){
    sendCmd("AT+CGREG?\r\n");				//Send CREG Command (Registration Status)
    readBuffer(respondBuffer, 36);			//Save CREG Reply to buffer
	return strstr(respondBuffer, "OK");		//Check that reply was good
}

bool DFRobot_SIM7000::checkAttachGPRS(void){
	return checkSendCmd("AT+CGATT?\r\n","CGATT: 1");
}


bool DFRobot_SIM7000::attachService(bool setting, uint16_t timeout){
  char gprsBuffer[36] = {0};
  sprintf(gprsBuffer, "AT+CGATT=%d\r\n", setting);	
  return checkSendCmd(gprsBuffer, "OK", timeout);
}

int DFRobot_SIM7000::checkSignalQuality(void)
{
  char respondBuffer[36] = {0};
  int i = 0,j = 0,k = 0;
  char *signalQuality;
  sendCmd("AT+CSQ\r\n");
  readBuffer(respondBuffer, 36);
  if(NULL != (signalQuality = strstr(respondBuffer, "+CSQ:"))){
    i = *(signalQuality + 6) - 48;
    j = *(signalQuality + 7) - 48;
    k =  (i * 10) + j;
  }else{
    return 0;
  }
  if( k == 99){
    return 0;
  }else{
    return k;
  }
}

// 0 - Minimum functionality mode (set the RF off but keep the SIM power on, previous mode must not be offline)
// 1 - Online mode (default value) (previous mode must not be offline)
// 4 - Offline mode (previous mode must not be FTM)
// 5 - Offline FTM (previous mode must be online)
// 6 - Reset MS (previous mode must be offline)
// 7 - RFoff mode (about radio frequency)
bool DFRobot_SIM7000::setFunctionality(uint8_t mode){
	char dataBuffer[36] = {0};					//Create buffer with 0x00 values
	sprintf(dataBuffer, "AT+CFUN=%d\r\n", mode);	//Generate command and save to data buffer
	return checkSendCmd(dataBuffer,"OK");				
}

// 1 - CAT-M
// 2 - NB-IoT
// 3 - CAT-M and NB-IoT
bool DFRobot_SIM7000::setPreferredLTEMode(uint8_t mode) {
	char lteModeCmd [36];
	sprintf(lteModeCmd, "AT+CMNB=%d\r\n", mode);
	return checkSendCmd(lteModeCmd,"OK");
}

bool DFRobot_SIM7000::setOnlyLTE(void) {
	return checkSendCmd("AT+CNMP=38\r\n","OK");
}

bool DFRobot_SIM7000::setServiceProvider(char *apn){
	char serviceProviderCmd [36];
	sprintf(serviceProviderCmd, "AT+CSTT=\"%s\"\r\n", apn);
	return checkSendCmd(serviceProviderCmd,"OK");
}

bool DFRobot_SIM7000::enableService(uint16_t timeout){
	return checkSendCmd("AT+CIICR\r\n", "OK", timeout);
}

bool DFRobot_SIM7000::statusService(uint16_t timeout){
	sendCmd("AT+CIICR=?\r\n");
    _s->setTimeout(timeout);
    return(_s->find("OK"));
}

bool DFRobot_SIM7000::getIpAddress(char* ipBuffer){
	sendCmd("AT+CIFSR\r\n");
    readBuffer(ipBuffer, 36);
	return(!strstr(ipBuffer, "ERROR"));
}

bool DFRobot_SIM7000::getIMEI(char* imeiBuffer){
	sendCmd("AT+GSN\r\n");	
	readBuffer(imeiBuffer, 36);
	return(strstr(imeiBuffer, "OK"));
}

//--------------------------------------------------------------------------------------------//
//-------------------------------------SSL Configuration--------------------------------------//
//--------------------------------------------------------------------------------------------//
bool DFRobot_SIM7000::SSL_ConfigCert(char* rootCA, char* certName, char* keyName){
	char sendBuffer[36] = {0};
	sprintf(sendBuffer, "AT+CSSLCFG=\"CONVERT\",2,\"%s\"\r\n", rootCA);
	if(checkSendCmd(sendBuffer, "OK")){
		sprintf(sendBuffer, "AT+CSSLCFG=\"CONVERT\",1,\"%s\",\"%s\"\r\n", certName, keyName);
		return checkSendCmd(sendBuffer, "OK");
	}else{
		return false;
	}
}

bool DFRobot_SIM7000::MQTTS_SetCert(uint8_t certIndex, char* certName, char* keyName){
	char sendBuffer[36] = {0};
	sprintf(sendBuffer, "AT+SMSSL=%d,%s,%s\r\n");
	return checkSendCmd(sendBuffer, "OK");
}
//--------------------------------------------------------------------------------------------//
//--------------------------------------------------------------------------------------------//
//--------------------------------------------------------------------------------------------//

uint32_t DFRobot_SIM7000::Connect(char* protocol, char* url, int socket, uint32_t timeout){
	char sendBuffer[64] = {0};
	char recvBuffer[64] = {0};
	sprintf(sendBuffer, "AT+CIPSTART=\"%s\",\"%s\",%d\r\n", protocol, url, socket);
	if(checkSendCmd(sendBuffer, "OK", 5000)){
		uint32_t startTime = millis();
		uint32_t endTime = millis() + timeout;
		while(millis() < endTime){
			readBuffer(recvBuffer, 64);
			if(strstr(recvBuffer, "CONNECT OK") != NULL){
				return (millis() - startTime);
			}
		}
		return 0x00;
	}else{
		return 0x00;
	}
}

bool DFRobot_SIM7000::closeConnection(){
	return checkSendCmd("AT+CIPCLOSE\r\n","CLOSE OK");
}

uint32_t DFRobot_SIM7000::apiHologram_Send(char* deviceKey, char* data, char* tags, uint32_t timeout){
	char sendCmdBuffer[24] = {0};
	char payloadBuffer[128] = {0};
	char recvBuffer[128] = {0};
	
	int length = sprintf(payloadBuffer, "{\"k\":\"7oIiM[Mx\",\"d\":\"hello from SIM7000\", \"t\":\"TOPIC\"}\r\n\032", deviceKey, data, tags);
	sprintf(sendCmdBuffer, "AT+CIPSEND=%d\r\n", (length));
	if(!checkSendCmd(sendCmdBuffer, ">")){
		Serial.println("CIPSEND Failed");
		return 0x00;
	}
	if(checkSendCmd(payloadBuffer, "SEND OK", 5000)){
		uint32_t startTime = millis();
		uint32_t endTime = millis() + timeout;
		while(millis() < endTime){
			memset(recvBuffer, 0x00, 128);
			readBuffer(recvBuffer, 128);
			if(strstr(recvBuffer, "[0,0]") != NULL){
				return (millis() - startTime);
			}else if(strstr(recvBuffer, "CLOSED") != NULL){
				Serial.println("Failure, Connection Closed");
				return 0x00;
			}
		}
		return 0x00;
	}else{
		return 0x00;
	}
}

bool DFRobot_SIM7000::ToggleWebApp(uint8_t setting){
	char cmdBuffer [32] = {0};
	sprintf(cmdBuffer, "AT+CNACT=%d,\"hologram\"\r\n", setting);	//Generate Baud Rate Command
	return checkSendCmd(cmdBuffer, "OK", 10000);
}

bool DFRobot_SIM7000::MQTT_Config(char* paramTag, char* paramVal){
	char sendCmdBuffer [64] = {0};
	sprintf(sendCmdBuffer, "AT+SMCONF=\"%s\",\"%s\"\r\n", paramTag, paramVal);	//Generate Baud Rate Command
	return checkSendCmd(sendCmdBuffer, "OK", 1000);
}

bool DFRobot_SIM7000::MQTT_ConfigUrl(char* paramUrl, char* paramPort){
	char sendCmdBuffer [64] = {0};														//Buffer for generating MQTT Parameter Command
	sprintf(sendCmdBuffer, "AT+SMCONF=\"URL\",\"%s\",\"%s\"\r\n", paramUrl, paramPort);	//Generate Baud Rate Command
	return checkSendCmd(sendCmdBuffer, "OK", 1000);
}

uint32_t DFRobot_SIM7000::MQTT_Connect(uint16_t timeout){
	sendCmd("AT+SMCONN\r\n");	
	uint32_t startTime = millis();
	_s->setTimeout(timeout);
	if(_s->find("OK")){
			return (millis() - startTime);
	}
	return false;
}

bool DFRobot_SIM7000::MQTT_ConnectStatus(){
	char recvBuffer[64] = {};
	sendCmd("AT+SMSTATE?\r\n");
	readBuffer(recvBuffer, 64);
	return checkSendCmd("AT+SMSTATE?\r\n", "+SMSTATE: 1");	
}

bool DFRobot_SIM7000::MQTT_Disconnect(void){
	return checkSendCmd("AT+SMDISC\r\n", "OK");
}

bool DFRobot_SIM7000::MQTT_Subscribe(const char* topic, uint8_t QoS) {
	char cmdStr[32];
	sprintf(cmdStr, "AT+SMSUB=%s,%i\r\n", topic, QoS);	
	return checkSendCmd(cmdStr, "OK");
}

bool DFRobot_SIM7000::MQTT_SubscribeParse(char* keyBuffer, char* valBuffer, uint16_t timeout){
	int i = 0;
	_s->setTimeout(timeout);
	if(_s->find("SMSUB")){
		_s->setTimeout(timeout * 10);
		_s->find("{\"");
		_s->readBytesUntil('"', keyBuffer, 8);
		_s->find(':');
		_s->readBytesUntil('}', valBuffer, 10);
		return true;
	}else{
		keyBuffer[0] = 0x00;
	}
	return false;	
}


bool DFRobot_SIM7000::MQTT_Publish(const char* topic, char* message, uint16_t contentLength, uint8_t QoS, uint8_t retain) {
  char cmdStr[128];
  sprintf(cmdStr, "AT+SMPUB=\"%s\",%i,%i,%i\r\n", topic, contentLength, QoS, retain);
	if(checkSendCmd(cmdStr, ">")){
	  return(checkSendCmd(message, "OK", 5000));
	} 
  return false;
}


bool DFRobot_SIM7000::MQTT_PublishAsJson(const char* topic, float* values, bool* updated, uint8_t numKeys) {
  char cmdStr[64];
  uint8_t lastElement = 0;
  int messageLength = 2;
  for(uint8_t i = 0; i < numKeys; ++i){
	 if(updated[i]){
		char key[8] = {};
		char val[16] = {};
		char keyValue[24] = {};
		dtostrf(values[i], 8, 2, val);
		EEPROM.get((i * 6), key);
		messageLength = messageLength + sprintf(keyValue, "\"%s\":%s,", key, val);
		lastElement = i;
		Serial.println(keyValue);
	 }
	}
  sprintf(cmdStr, "AT+SMPUB=\"%s\",%d,1,0\r\n", topic, messageLength);
  if(checkSendCmd(cmdStr, ">", 1000)){
	  _s->write("{");	
	for(uint8_t i = 0; i < numKeys; ++i){
		if(updated[i]){
			char key[8] = {};
			char val[16] = {};
			char keyValue[24] = {};
			dtostrf(values[i], 8, 2, val);
			EEPROM.get((i * 6), key);
			if(i == lastElement){
				sprintf(keyValue, "\"%s\":%s", key, val);
			}else{
				sprintf(keyValue, "\"%s\":%s,", key, val);
			}
			_s->write(keyValue);
		}
	}
	_s->write("}\r\n\032");
	return true;
  }
  return false;
}

bool DFRobot_SIM7000::MQTT_PublishAsPbuf(const char* topic, float* values, bool* updated, uint8_t numVals) {
	char cmdStr[64];
	int messageLength = 6; 
	sprintf(cmdStr, "AT+SMPUB=\"%s\",%d,1,0\r\n", topic, messageLength);
	if(checkSendCmd(cmdStr, ">", 2500)){
		_s->write(0x0D);
		_s->write((uint8_t)0x00);
		_s->write((uint8_t)0x00);
		_s->write(0x16);
		_s->write(0x43);
		return (checkSendCmd("\r\n\032", "OK", 20000));
	}	
  return false;
}

bool DFRobot_SIM7000::MQTT_PublishJsonVal(const char* topic, char* jsonKey, int jsonVal, uint16_t timeout) {
  char cmdStr[128];
  char sendStr[36];
  int messageLength = sprintf(sendStr, "{\"%s\":%d}\r\n\032", jsonKey, jsonVal) - 2;
  sprintf(cmdStr, "AT+SMPUB=\"%s\",%d,1,0\r\n", topic, messageLength);
  _s->setTimeout(1000);
  sendCmd(cmdStr);
  if(_s->find(">")){
	  _s->setTimeout(timeout);
	  sendCmd(sendStr);
	  if(_s->find("OK")){
			return true;
	  }
  }
  return false;
}

bool DFRobot_SIM7000::send(char *data)
{
  char num[4];
  char resp[20];
  int len = strlen(data);
  itoa(len, num, 10);
  sendCmd("AT+CIPSEND=");
  sendCmd(num);
  if(checkSendCmd("\r\n",">")){
    sendCmd(data);
    while(1){
      while(checkReadable()){
        readBuffer(resp,20);
        if(NULL != strstr(resp,"OK")){
          return true;
        }
        if(NULL != strstr(resp,"ERROR")){
          return false;
        }
      }
    }
  }else{
    return false;
  }
}

bool DFRobot_SIM7000::send(char *buf, size_t len)
{
  char num[4];
  itoa(len, num, 10);
  sendCmd("AT+CIPSEND=");
  sendCmd(num);
  if(checkSendCmd("\r\n",">")){
    if(checkSendCmd(buf,"OK")){
      return true;
    }else{
      return false;
    }
  }else{
    return false;
  }
}


bool DFRobot_SIM7000::closeNetwork(void)
{
  return checkSendCmd("AT+CIPSHUT\r\n","SHUT OK", 5000);
}

int DFRobot_SIM7000::batteryPower(void)
{
  char batteryBuff[26] = {0x00};
  char *pBattery;
  int i=0,j=0,k=0;
  sendCmd("AT+CBC\r\n");
  readBuffer(batteryBuff,26);
  pBattery = strstr(batteryBuff,"+CBC:");
  i = *(pBattery + 8) - 48;
  j = *(pBattery + 9) - 48;
  k =  (i * 10) + j;
  return  k;
}