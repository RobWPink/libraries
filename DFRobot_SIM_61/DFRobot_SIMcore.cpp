#include <DFRobot_SIMcore.h>

DFRobot_SIMcore::DFRobot_SIMcore(Stream *s)
{
  _s = s;
}

bool DFRobot_SIMcore::turnOFF(void)
{
  if(checkSendCmd("AT+CPOWD=1\r\n","NORMAL POWER DOWN")){
    closeCommand();
    return true;
  }else{
    return false;
  }
}

void DFRobot_SIMcore::setPhoneNumber(char *n)
{
  _phoneNumber = n;
}

char* DFRobot_SIMcore::getPhoneNumber(void)
{
  return _phoneNumber;
}

void DFRobot_SIMcore::setPort(int p)
{
  _port = p;
}

int16_t DFRobot_SIMcore::getPort(void)
{
  return _port;
}

void DFRobot_SIMcore::setCommandCounter(uint8_t c)
{
  _commandCounter = c;
}

uint8_t DFRobot_SIMcore::getCommandCounter(void)
{
  return _commandCounter;
}

void DFRobot_SIMcore::setCommandError(uint8_t n)
{
  _commandError = n;
}

uint8_t DFRobot_SIMcore::getCommandError(void)
{
  return _commandError;
}

void DFRobot_SIMcore::setOngoingCommand(eSIMCommand_t c)
{
  _ongoingCommand = c;
}

DFRobot_SIMcore::eSIMCommand_t DFRobot_SIMcore::getOngoingCommand(void)
{
  return _ongoingCommand;
}

void DFRobot_SIMcore::openCommand(eSIMCommand_t c)
{
  setCommandError(0);
  setCommandCounter(1);
  setOngoingCommand(c);
}

void DFRobot_SIMcore::closeCommand()
{
  setCommandError(0);
  setCommandCounter(0);
  setOngoingCommand(eNONE);
}

int DFRobot_SIMcore::checkReadable(void)
{
  return _s->available();
}

void DFRobot_SIMcore::sendBuff(const char* buff,size_t num)
{
  _s->write(buff,num);
}

void DFRobot_SIMcore::sendString(const char* buff)
{
  DBG(buff);
  _s->print(buff);
}  

bool DFRobot_SIMcore::checkSendCmd(const char* cmd, const char* resp, uint32_t timeout)
{
  sendCmd(cmd);
  _s->setTimeout(timeout);
  return(_s->find((char*)resp));
}

uint16_t DFRobot_SIMcore::readBuffer(char* buffer, uint16_t bufferSize, uint32_t timeout = 2500)
{
  _s->setTimeout(timeout);
  return _s->readBytes(buffer, bufferSize);
}

void DFRobot_SIMcore::sendCmd(const char* cmd)
{
  _s->write(cmd);			//Send Command
}

void DFRobot_SIMcore::get_String(void)
{
    uint64_t t1 = millis();
    while(1){
        if(checkReadable()){
            Serial.print(char(_s->read()));
            t1 = millis();
        }
        if((millis() - t1) > 10000){
            return;
        }
    }
}
