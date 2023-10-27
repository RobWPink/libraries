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

bool DFRobot_SIM7000::restart(uint8_t powerPin, uint8_t resetPin){
	pinMode(resetPin,OUTPUT);						//Initialize Pin for resetting the modem
	digitalWrite(resetPin, LOW);					//Immediately set LOW, reset requires a 200ms strobe of reset pin
	delay(500);
	digitalWrite(resetPin, HIGH);					//Set back to HIGH
	return start(powerPin, resetPin, 1000);
}

bool DFRobot_SIM7000::setBaudRate(int32_t rate){		//Toggle baudrate of modem serial port
  char baudChangeCmd [18] = {0};					//Buffer for generating baud rate command
  sprintf(baudChangeCmd, "AT+IPR=%d\r\n", rate);	//Generate Baud Rate Command
  return checkSendCmd(baudChangeCmd,"OK");			//Send Baud Rate Command
}

bool DFRobot_SIM7000::setRiPin(int setting){
	char baudChangeCmd [14] = {0};					//Buffer for generating baud rate command
	sprintf(baudChangeCmd, "AT+CFGRI=%d\r\n", setting);	//Generate Baud Rate Command
	return checkSendCmd(baudChangeCmd,"OK");		//Send Baud Rate Command
}

bool DFRobot_SIM7000::checkForModem(void){
	return(checkSendCmd("AT\r\n", "OK", 500));
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

void DFRobot_SIM7000::getTimestamp(char* timeStampBuffer){
	sendCmd("AT+CLTS=?\r\n");
	readBuffer(timeStampBuffer,64, 2500);
}

bool DFRobot_SIM7000::getTime(tm* timeBuffer){
	if(checkSendCmd("AT+CCLK?\r\n","+CCLK: ")){
		_s->find('"');
		timeBuffer->tm_year = _s->parseInt();
		_s->read();
		timeBuffer->tm_mon = _s->parseInt();
		_s->read();
		timeBuffer->tm_mday = _s->parseInt();
		_s->read();
		timeBuffer->tm_hour = _s->parseInt();
		_s->read();
		timeBuffer->tm_min = _s->parseInt();
		_s->read();
		timeBuffer->tm_sec = _s->parseInt();
		return true;
	}
	return false;
}

int DFRobot_SIM7000::checkNTPStatus(void){
	if(checkSendCmd("AT+CNTP\r\n","+CNTP: ")){
		return _s->parseInt();
	}else{
		return false;
	}
}

bool DFRobot_SIM7000::bearerProfile_Configure(int profile, const char* parameter, const char* value){
	char bearerConfigureCmd [64] = {0};	
	sprintf(bearerConfigureCmd, "AT+SAPBR=3,%d,%s,%s\r\n", profile, parameter, value);
	return checkSendCmd(bearerConfigureCmd,"OK");
}

bool DFRobot_SIM7000::bearerProfile_Open(int profile){
	char bearerOpenCmd [64] = {0};	
	sprintf(bearerOpenCmd, "AT+SAPBR=1,%d\r\n", profile);
	return checkSendCmd(bearerOpenCmd,"OK");
}

bool DFRobot_SIM7000::NTP_Configure(const char* ntpUrl, int ntpTimezone, int cid, int ntpMode){
	char ntpConfigureCmd [64] = {0};	
	sprintf(ntpConfigureCmd, "AT+CNTPCID=%d\r\n", cid);
	if(!checkSendCmd(ntpConfigureCmd,"OK")){
		return false;
	}
	snprintf(ntpConfigureCmd, 64, "AT+CNTP=\"%s\",%d,%d,%d\r\n", ntpUrl, ntpTimezone, cid, ntpMode);	
	if(!checkSendCmd(ntpConfigureCmd,"OK")){
		return false;
	}
	return checkSendCmd("AT+CNTP\r\n","OK");
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
		delay(100);								//Wait between retries
	}
	return false;
}

bool DFRobot_SIM7000::getIpState(char* statusBuffer){
	sendCmd("AT+CIPSTATUS\r\n");
	readBuffer(statusBuffer,32, 2500);
	return(strstr(statusBuffer, "OK"));
}

bool DFRobot_SIM7000::checkNetStatus(char* respondBuffer){
    sendCmd("AT+CGREG?\r\n");				//Send CREG Command (Registration Status)
    readBuffer(respondBuffer, 32, 2500);			//Save CREG Reply to buffer
	return strstr(respondBuffer, "OK");		//Check that reply was good
}

bool DFRobot_SIM7000::checkAttachGPRS(void){
	return checkSendCmd("AT+CGATT?\r\n","CGATT: 1");
}


bool DFRobot_SIM7000::attachService(bool setting, uint16_t timeout){
  char gprsBuffer[32] = {0};
  sprintf(gprsBuffer, "AT+CGATT=%d\r\n", setting);	
  return checkSendCmd(gprsBuffer, "OK", timeout);
}

int DFRobot_SIM7000::checkSignalQuality(void){
  if(checkSendCmd("AT+CSQ\r\n","+CSQ: ")){
		return _s->parseInt();
	}else{
		return false;
	}
}

// 0 - Minimum functionality mode (set the RF off but keep the SIM power on, previous mode must not be offline)
// 1 - Online mode (default value) (previous mode must not be offline)
// 4 - Offline mode (previous mode must not be FTM)
// 5 - Offline FTM (previous mode must be online)
// 6 - Reset MS (previous mode must be offline)
// 7 - RFoff mode (about radio frequency)
bool DFRobot_SIM7000::setFunctionality(uint8_t mode){
	char dataBuffer[18] = {0};					//Create buffer with 0x00 values
	sprintf(dataBuffer, "AT+CFUN=%d\r\n", mode);	//Generate command and save to data buffer
	return checkSendCmd(dataBuffer,"OK");				
}

// 1 - CAT-M
// 2 - NB-IoT
// 3 - CAT-M and NB-IoT
bool DFRobot_SIM7000::setPreferredLTEMode(uint8_t mode) {
	char lteModeCmd [18] = {0};
	sprintf(lteModeCmd, "AT+CMNB=%d\r\n", mode);
	return checkSendCmd(lteModeCmd,"OK");
}

bool DFRobot_SIM7000::setOnlyLTE(uint8_t mode) {
	char cmd[18] = {0};
	sprintf(cmd, "AT+CNMP=%d\r\n", mode);
	return checkSendCmd(cmd,"OK");
}

bool DFRobot_SIM7000::setServiceProvider(const char *apn){
	char cmd[36] = {0};
	snprintf(cmd, sizeof(cmd), "AT+CSTT=\"%s\"\r\n", apn);
	return checkSendCmd(cmd,"OK");
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
    readBuffer(ipBuffer, 32, 2500);
	return(!strstr(ipBuffer, "ERROR"));
}

bool DFRobot_SIM7000::getIMEI(char* imeiBuffer){
	sendCmd("AT+GSN\r\n");	
	readBuffer(imeiBuffer, 32, 2500);
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
			readBuffer(recvBuffer, 64, 2500);
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
			readBuffer(recvBuffer, 128, 2500);
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

bool DFRobot_SIM7000::ToggleWebApp(uint8_t setting, const char *apn){
	char cmd[32] = {0};
	snprintf(cmd, sizeof(cmd), "AT+CNACT=%d,\"%s\"\r\n", setting, apn);	//Generate Baud Rate Command
	return checkSendCmd(cmd, "OK", 10000);
}

bool DFRobot_SIM7000::MQTT_Config(const char* tag, const char* val){
	char cmd[64] = {0};
	snprintf(cmd, sizeof(cmd), "AT+SMCONF=\"%s\",\"%s\"\r\n", tag, val);	
	return checkSendCmd(cmd, "OK", 1000);
}

bool DFRobot_SIM7000::MQTT_ConfigUrl(char* url, char* port){
	char cmd[64] = {0};												
	snprintf(cmd, sizeof(cmd),"AT+SMCONF=\"URL\",\"%s\",\"%s\"\r\n", url, port);	
	return checkSendCmd(cmd, "OK", 1000);
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

bool DFRobot_SIM7000::MQTT_ConnectStatus(uint32_t timeout){
	uint32_t endTime = millis() + timeout;
	while(millis() < endTime){
		if(checkSendCmd("AT+SMSTATE?\r\n", "+SMSTATE: 1", 50)){
			return true;
		}
	}
	return false;	
}

bool DFRobot_SIM7000::MQTT_Disconnect(void){
	return checkSendCmd("AT+SMDISC\r\n", "OK");
}

bool DFRobot_SIM7000::MQTT_Subscribe(const char* topic, int QoS) {
	char cmd[64];
	snprintf (cmd, 64, "AT+SMSUB=\"%s\",%i\r\n", topic, QoS);	
	return checkSendCmd(cmd, "OK", 20000);
}

bool DFRobot_SIM7000::MQTT_Unsubscribe(const char* topic) {
	char cmd[64];
	snprintf (cmd, 64, "AT+SMUNSUB=\"%s\"\r\n", topic);	
	return checkSendCmd(cmd, "OK", 20000);
}

int DFRobot_SIM7000::MQTT_SubscribeParse(char* keyBuffer, char* valBuffer, uint16_t timeout){
	_s->setTimeout(timeout);
	if(_s->find("+SMSUB")){
		_s->setTimeout(timeout * 10);
		_s->find('{');							//Start of Key-Value Pair
		
		_s->find('"');							//Start of Key
		_s->readBytesUntil(6, keyBuffer, 5);	
		keyBuffer[5] = '\0';					//Add null terminator
		
		_s->find(':');							//Start of Value
		valBuffer[_s->readBytesUntil('}', valBuffer, 7)] = '\0';
		
		return 0x01;
	}else{
		keyBuffer[0] = 0x00;
	}
	return 0x00;	
}


bool DFRobot_SIM7000::MQTT_Publish(const char* topic, char* message, uint16_t contentLength, uint8_t QoS, uint8_t retain) {
  char cmd[128];
  snprintf(cmd, sizeof(cmd), "AT+SMPUB=\"%s\",%i,%i,%i\r\n", topic, contentLength, QoS, retain);
	if(checkSendCmd(cmd, ">")){
	  return(checkSendCmd(message, "OK", 5000));
	} 
  return false;
}



bool DFRobot_SIM7000::MQTT_PublishAsJson(const char* topic, float* values, bool* updated, uint8_t numKeys) {
  char cmdStr[64];
  char keyValue[24] = {0};
  uint8_t lastElement = 0;
  int messageLength = 2;
  int keyCount = 0;
  for(uint8_t i = 0; i < numKeys; ++i){
	 if(updated[i]){
		char key[10] = {};
		char val[12] = {};
		dtostrf(values[i], 8, 2, val);
		EEPROM.get((i * 6), key);
		messageLength = messageLength + snprintf(keyValue, sizeof(keyValue), "\"%s\":%s,", key, val);
		lastElement = i;
	 }
	}
  sprintf(cmdStr, "AT+SMPUB=\"%s\",%d,1,0\r\n", topic, messageLength);
  if(checkSendCmd(cmdStr, ">", 1000)){
	  _s->write("{");	
	for(uint8_t i = 0; i < numKeys; ++i){
		if(updated[i]){
			updated[i] = false;
			char key[10] = {};
			char val[12] = {};
			dtostrf(values[i], 8, 2, val);
			EEPROM.get((i * 6), key);
			if(i == lastElement){
				 snprintf(keyValue, sizeof(keyValue), "\"%s\":%s", key, val);
				 _s->write(keyValue);
				 _s->write("}\r\n\032");
				 return true;
			}else{
				 snprintf(keyValue, sizeof(keyValue), "\"%s\":%s,", key, val);
				 _s->write(keyValue);
			}	
			Serial.println(keyValue);
		}
	}
	return true;
  }
  return false;
}

bool DFRobot_SIM7000::MQTT_PublishAsPbuf(const char* topic) {
	char cmdStr[64];
	bool success = 0;
	int messageLength = 10; 
	sprintf(cmdStr, "AT+SMPUB=\"%s\",%d,1,0\r\n", topic, messageLength);
	if(checkSendCmd(cmdStr, ">", 2500)){
		success = checkSendCmd("\x0D\x00\x00\x40\x40\r\n\032", "OK", 20000);
	}
  return success;
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
//----------------------------------------------------------//
//---------------------------GNSS---------------------------//
//----------------------------------------------------------//
bool DFRobot_SIM7000::GNSS_Toggle(bool setting){
	char cmd[24];
	sprintf(cmd, "AT+CGNSPWR=%d\r\n", setting);
	return checkSendCmd(cmd, "OK");
}

bool DFRobot_SIM7000::GNSS_AT_Set(bool enableAT, int packetCount){
	char cmd[24];
	sprintf(cmd, "AT+CGNSTST=%d,%d\r\n", enableAT, packetCount);
	return checkSendCmd(cmd, "OK");
}
  
//----------------------------------------------------------//
//----------------------------------------------------------//
//----------------------------------------------------------//
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
        readBuffer(resp,20, 2500);
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
    return(checkSendCmd(buf,"OK"));
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
  readBuffer(batteryBuff,26, 2500);
  pBattery = strstr(batteryBuff,"+CBC:");
  i = *(pBattery + 8) - 48;
  j = *(pBattery + 9) - 48;
  k =  (i * 10) + j;
  return  k;
}