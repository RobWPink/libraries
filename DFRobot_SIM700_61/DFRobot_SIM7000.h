/*!
 * @file DFRobot_SIM7000.h
 * @brief Define the basic structure of DFRobot_SIM7000 class
 * @details This is a stable low-power communication module, and it supports multiple frequency bands of LTE- tdd / LTE- fdd / GSM / GPRS / EDGE. The upload and download data flow peak at 375kbps.
 * @copyright	Copyright (c) 2021 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license     The MIT License (MIT)
 * @author [TangJie](jie.tang@dfrobot.com)
 * @version  V1.0.1
 * @date  2021-09-16
 * @url https://github.com/DFRobot/DFRobot_SIM7000
 */                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                
#ifndef __DFROBOT_SIM7000_H__
#define __DFROBOT_SIM7000_H__

#include "DFRobot_SIMcore.h"

#include "Arduino.h"
#include "String.h"
#include "EEPROM.h"

#define ON  0
#define OFF 1


#if 0
#define DBG(...) {Serial.print("["); Serial.print(__FUNCTION__); Serial.print("(): "); Serial.print(__LINE__); Serial.print(" ] "); Serial.println(__VA_ARGS__);}
#else
#define DBG(...)
#endif

class DFRobot_SIM7000 : public DFRobot_SIMcore{
public:
  /**
   * @enum eProtocol
   * @brief Select Network communication protocol
   */
  typedef enum  {
    eCLOSED = 0,
    eTCP    = 1,
    eUDP    = 2,
  }eProtocol;

  /**
   * @enum eNet
   * @brief Select data communication mode
   */
  typedef enum {
    eGPRS,
    eNB,
  }eNet;

public:
  /**
   * @fn DFRobot_SIM7000
   * @brief DFRobot_SIMcore constructor of abstract class. Construct serial ports.
   * @param s The pointer to abstract class, where you can fill in the pointer to serial object.
   * @return None
   */
  DFRobot_SIM7000(Stream *s);
  ~DFRobot_SIM7000(){};

  /**
   * @fn recv
   * @brief Receive
   * @param buf Receive data content
   * @param maxlen Receive data length
   * @return Get data length
   */
  int recv(char* buf, int maxlen);
 
  /**
   * @fn checkSignalQuality
   * @brief Check signal quality
   * @return 0-30:Signal quality
   */
  int checkSignalQuality(void);

  /**
   * @fn batteryPower
   * @brief Battery power
   * @return Battery power
   */
  int batteryPower(void);
 

  /**
   * @fn attachService
   * @brief Open the connection
   * @return bool type, indicating the status of opening the connection
   * @retval true Success 
   * @retval false Failed
   */
  bool attachService(bool setting, uint16_t timeout);
  
  /**
   * @fn checkForModem
   * @brief Check if an AT Modem is present with standard AT Command
   * @return bool type, indicating the modem AT interface is available
   * @retval true Success 
   * @retval false Failed
   */
  bool checkForModem(void);

  /**
   * @fn setBaudRate
   * @brief Set baud rate to avoid garbled
   * @param rate Baud rate value
   * @n    Possible values:1200 2400 4800 9600 19200 38400
   * @note SIM7000 default baud rate is 115200, reduce the baud rate to avoid distortion
   * @return bool type, indicating the status of setting
   * @retval true Success 
   * @retval false Failed
   */
  bool setBaudRate(int rate);
  
  /**
   * @fn toggleEcho
   * @brief Toggle if command echoing is on or off
   * @return bool type, indicating if echo was toggled
   * @retval true Success 
   * @retval false Failed
   */
  bool toggleEcho(bool setting);

  /**
   * @fn checkSIMStatus
   * @brief Check SIM card
   * @return bool type, indicating the status of checking SIM card
   * @retval true Success 
   * @retval false Failed
   */
  bool checkSIMStatus(void);
  
  /**
   * @fn checkNetStatus
   * @brief Check network registration status. Results are stored in cregReply
   * @param cregReply: Buffer to store reply from CREG command
   * @return bool type, indicates if status check was successful
   * @retval true Success 
   * @retval false Failed
   */
  bool checkNetStatus(char* cgregReply);
  
  /**
   * @fn waitForIpState
   * @brief Check network registration status. Results are stored in cregReply
   * @param cregReply: Buffer to store reply from CREG command
   * @return bool type, indicates if status check was successful
   * @retval true Success 
   * @retval false Failed
   */
  bool waitForIpState(const char* desiredState, uint32_t timeout);
  
   /**
   * @fn setOnlyLTE
   * @brief Set modem to only use LTE
   * @return bool type, inidicating if the modification was a success
   * @retval true Success 
   * @retval false Failed
   */
  bool setOnlyLTE(void);
  
  /**
   * @fn setFunctionality
   * @brief Set modem to allow full functionality
   * @return bool type, inidicating if the modification was a success
   * @retval true Success 
   * @retval false Failed
   */
  bool setFunctionality(uint8_t mode);
  
  /**
   * @fn setPreferredLTEMode
   * @brief Select preferred cellular network type.
   * @param 1 = CAT-M, 2=NB-IoT, 3=BOTH
   * @return bool type, inidicating if the modification was a success
   * @retval true Success 
   * @retval false Failed
   */
  bool setPreferredLTEMode(uint8_t mode);

  /**
   * @fn getConnectionStatus
   * @brief Obtain the TCP/UDP/SSL Connection Status and Information
   * @return bool type, inidicating if the retrieval was a success
   * @retval true Success 
   * @retval false Failed
   */
  bool getIpState(char* statusBuffer);
  
  
  /**
   * @fn connect
   * @brief Connect to a URL over a given socket using a given protocol (TCP/UDP)
   * @param protocol = TCP or UDP
   * @param url = URL of the site you are contacting
   * @param port = The port available for access to the site
   * @return uint32_t type, inidicating length of time for the connection.
   * @retval SUCCESS returns Length of time for connection 
   * @retval FAILURE returns 0x00
   */
  uint32_t Connect(char* protocol, char* url, int socket, uint32_t timeout);

  /**
   * @fn closeConnection
   * @brief close active connection
   * @return bool type, inidicating if modification was successful
   * @retval SUCCESS returns Length of time for connection 
   * @retval FAILURE returns 0x00
   */  
  bool closeConnection();
  
  /**
   * @fn apiHologram_Send
   * @brief Send a message to the hologram cloud
   * @param deviceKey = Retrieved from hologram cloud webhooks page for a device
   * @param data = payload of message you want to send
   * @param length = length of payload
   * @param tags = hologram cloud tags
   * @param timeout = how long to attempt sending
   * @return uint32_t type, inidicating length of time for the connection.
   * @retval SUCCESS returns Length of time for connection 
   * @retval FAILURE returns 0x00
   */
  uint32_t apiHologram_Send(char* deviceKey, char* data, char* tags, uint32_t timeout);
  
  /**
   * @fn getIMEI
   * @brief Obtain the IMEI of the inserted SIM card
   * @return bool type, inidicating if the retrieval was a success
   * @retval true Success 
   * @retval false Failed
   */
  bool getIMEI(char* imeiBuffer);
  
   /**
   * @fn setServiceProvider
   * @brief Set the APN/Service Provider for your Modem. Examples: 'hologram', 'verizon'
   * @return bool type, inidicating if the modification was a success
   * @retval true Success 
   * @retval false Failed
   */
  bool setServiceProvider(char *apn);
  
  /**
  * @fn enableService
  * @brief Enable connection to the service provider. SIM Card should be registered and valid before attemting.
  * @return bool type, inidicating if the modification was a success
  * @retval true Success 
  * @retval false Failed
  */ 
  bool enableService(uint16_t timeout);
  
   /**
   * @fn getIpAddress
   * @brief Retrieve the IP Address assigned by the service provider. Should be called AFTER enableService
   * @return bool type, inidicating if the retrieval was a success
   * @retval true Success 
   * @retval false Failed
   */
  bool getIpAddress(char* ipBuffer);

  /**
   * @fn closeNetwork
   * @brief End the connection
   * @return bool type, indicating the status of closing Network
   * @retval true Success 
   * @retval false Failed
   */
  bool closeNetwork(void);

  /**
   * @fn start
   * @brief Turn ON SIM7000 and verify communication is good
   * @param powerPin The pin connected to SIM7000 Power Key pin
   * @param resetPin The pin connected to SIM7000 Reset Key pin
   * @return bool type, indicating the status of turning on
   * @retval true Success 
   * @retval false Failed
   */
  bool start(uint8_t powerPin, uint8_t resetPin, uint32_t timeout);
  
  
  /**
   * @fn restart
   * @brief Turn ON SIM7000 and verify communication is good
   * @param powerPin The pin connected to SIM7000 Power Key pin
   * @param resetPin The pin connected to SIM7000 Reset Key pin
   * @return bool type, indicating the status of turning on
   * @retval true Success 
   * @retval false Failed
   */
  bool restart(uint8_t powerPin, uint8_t resetPin, uint32_t timeout);

  /**
   * @fn GNSS_Enable
   * @brief Init SIM7000 positioning module
   * @return bool type, indicating the initialization status
   * @retval true Success 
   * @retval false Failed
   */
  bool GNSS_Enable(void);
  
  /**
   * @fn MQTT_Config
   * @brief Set MQTT Parameters
   * @return bool type, inidicating if the modification was a success
   * @retval true Success 
   * @retval false Failed
   */
  bool MQTT_Config(char* paramTag, char* paramVal);
  
  /**
   * @fn MQTT_ConfigUrl
   * @brief Set MQTT URL
   * @return bool type, inidicating if the modification was a success
   * @retval true Success 
   * @retval false Failed
   */
  bool MQTT_ConfigUrl(char* paramUrl, char* paramPort);
  
  /**
   * @fn MQTT_Connect
   * @brief Connect to MQTT Server
   * @return bool type, inidicating if the connection was a success
   * @retval true Success 
   * @retval false Failed
   */
  uint32_t MQTT_Connect(uint16_t timeout);
  
  /**
   * @fn MQTT_ConnectStatus
   * @brief Get status of MQTT Connection
   * @return bool type, inidicating if the connection is good
   * @retval true Success 
   * @retval false Failed
   */
  bool MQTT_ConnectStatus();
  
  bool ToggleWebApp(uint8_t setting);
  bool MQTT_Subscribe(const char* topic, uint8_t QoS);
  bool MQTT_SubscribeParse(char* keyBuffer, char* valBuffer, uint16_t timeout);
  bool MQTT_GetAttributes(const char* topic, uint16_t timeout);
  bool MQTT_Publish(const char* topic, char* message, uint16_t contentLength, uint8_t QoS, uint8_t retain);
  bool MQTT_PublishJsonVal(const char* topic, char* jsonKey, int jsonVal, uint16_t timeout);
  bool MQTT_PublishAsJson(const char* topic, float *values, bool* updated, uint8_t numKeys);
  bool MQTT_PublishAsPbuf(const char* topic, float* values, bool* updated, uint8_t numVals);
  bool toggleErrorReporting(uint8_t setting);
  bool checkAttachGPRS(void);
  bool setRiPin(int setting);
  bool statusService(uint16_t timeout);
  bool SSL_ConfigCert(char* rootCA, char* certName, char* keyName);
  bool MQTTS_SetCert(uint8_t certIndex, char* certName, char* keyName);
  
  /**
   * @fn MQTT_Disconnect
   * @brief MQTT_Disconnect from MQTT Server
   * @return bool type, inidicating if the disconnection was a success
   * @retval true Success 
   * @retval false Failed
   */
  bool MQTT_Disconnect(void);

  /**
   * @fn httpGet
   * @brief HTTP GET This function print the get data
   * @param host URL
   */
  void httpGet(const char *host);

  /**
   * @fn httpDisconnect
   * @brief Disconnect from server and cancel initialization
   */
  bool httpDisconnect(void);

  /**
   * @fn send
   * @brief Send data with specify the length
   * @param buf The buffer for data to be send
   * @param len The length of data to be send
   * @return bool type, indicating status of sending
   * @retval true Success 
   * @retval false Failed
   */
  bool send(char *buf,size_t len);

  /**
   * @fn send
   * @brief Send data
   * @param data The data to send
   * @return bool type, indicating status of sending
   * @retval true Success 
   * @retval false Failed
   */
  bool send(char *data);

  /**
   * @fn getPosition
   * @brief Get the current position
   * @return bool type, indicating the status of getting position
   * @retval true Success 
   * @retval false Failed
   */
  bool getPosition(void);

  /**
   * @fn getLatitude
   * @brief Get latitude
   * @return Latitude value
   */
  const char* getLatitude(void);

  /**
   * @fn getLongitude
   * @brief Get longitude
   * @return Longitude value
   */
  const char* getLongitude(void);
  
private:
	char _latitude[8];
	char _longitude[8];
	int  _mode_t=0;
	long _baudrate = 19200;
	uint8_t _resetPin = 0;
	uint8_t _powerPin = 0;
	Stream *_s;
 };

#endif
