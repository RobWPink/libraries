/**
 * @file bronkhorstFM.cpp
 * @brief Interface for the Bronkhorst Flow Meter
 * @author Christopher Long, ONEH2
 * @date May 19, 2022
 * @version 0.0

 *last modified 06/24 by Sandeep
 */

#include <bronkhorstFM.h>

// Commands
#define WR_FLOWUNIT     ":0780000100000F32\n\r"
#define RD_FLOWUNIT     ":0680000100000F\n\r"
#define RD_SETPOINT     ":06800401210121\r\n"
#define RD_MEASURE      ":06800401210120\r\n"
#define RD_CONTROL_MODE ":06800401040104\r\n"
#define RD_FMEASURE     ":06800421402140\r\n"
#define RD_FSETPOINT    ":06800421412143\r\n"
#define RD_TEMP         ":06800421472147\r\n"
#define RD_VALVE_OUT    ":06800472417241\r\n"
#define RD_ACT_DENSITY  ":068004744F744F\r\n"
#define RD_SERIAL_NUM   ":0780047163716300\r\n"
#define RD_MODEL_NUM    ":0780047162716200\r\n"
#define RD_FIRMWARE_VER ":0780047165716506\r\n"
#define RD_DEVICE_TYPE  ":0780047161716106\r\n"

Bronkhorst_FM::Bronkhorst_FM(Stream &s)
{
    _s = &s;    
}

void Bronkhorst_FM::wr_flowunit_kg_h(void){
	_s->print(WR_FLOWUNIT);
}

int Bronkhorst_FM::rd_controlmode(void)
{
    static int mode = 0;
    
    _s->print(RD_CONTROL_MODE);
    receiveFMPacket();
    if (mode != fm_param._mode)
    {
        mode = fm_param._mode;
    }

    return mode;
};

int Bronkhorst_FM::rd_setpoint(void)
{
    static int setpoint = 0;
    fm_param.rd_stpnt_flg = 1;

    _s->print(RD_SETPOINT);
    receiveFMPacket();
    if (setpoint != fm_param._setpoint)
    {
        setpoint = fm_param._setpoint;
    }    

    return setpoint;
};

int Bronkhorst_FM::rd_measure(void)
{
    static int measure = 0;    

    _s->print(RD_MEASURE);
    receiveFMPacket();
    if ((measure != fm_param._measure) && fm_param.rd_stpnt_flg != 1)
    {
        measure = fm_param._measure;
    }

    return measure;
};

float Bronkhorst_FM::rd_fmeasure(void)
{
    static float fmeasure = 3.14;

    _s->print(RD_FMEASURE);
    receiveFMPacket();
    if (fmeasure != fm_param._fmeasure)
    {
        fmeasure = fm_param._fmeasure;
    }   

    return fmeasure;
};

float Bronkhorst_FM::rd_temp(void)
{
    static float temp = 3.14;

    _s->print(RD_TEMP);
    receiveFMPacket();        
    if (temp != fm_param._temp)
    {
        temp = fm_param._temp;
    }
    
    return temp;
};

long Bronkhorst_FM::rd_valOutput(void)
{
  static long valOutput = 3.14;

  _s->print(RD_VALVE_OUT);
  receiveFMPacket();
  if(valOutput != fm_param._valOutput)
  {
      valOutput = fm_param._valOutput;
  }

  return valOutput;
}

float Bronkhorst_FM::rd_actDensity(void)
{
  static float actDensity = 0;

  _s->print(RD_ACT_DENSITY);
  receiveFMPacket();
  if (actDensity != fm_param._actDensity)
  {
    actDensity = fm_param._actDensity;
  }

  return actDensity;
}



int Bronkhorst_FM::wr_controlmode(int mode)
{
    static int response = -1;
    char buf[64] = {0};

    sprintf(buf,":0580010104%X\n\r", mode);
    _s->print(buf);
    receiveFMPacket();
    
    return response;
};

int Bronkhorst_FM::wr_setpoint(int setpoint)
{
    static int response = 0;
    char buf[64] = {0};

    sprintf(buf,":0603010121%X\n\r", setpoint);
    _s->print(buf);
    receiveFMPacket();      

    return response;
};

void Bronkhorst_FM::receiveFMPacket(void)
{    
    static bool recvInProgress = false;
    static byte ndx = 0;
    char pkt_start = ':';
    char pkt_end = '\r';
    char value = 0;

    while (_s->available() > 0)
    {
        value = _s->read();
        if (recvInProgress == true)
        {
            if (value != pkt_end)
            {
                _flowBuffer[ndx] = value;
                ndx++;
                if (ndx >= PACKET_SIZE)
                {
                    ndx = PACKET_SIZE - 1;
                }
            }
            else
            {
                _flowBuffer[ndx] = '\0';
                recvInProgress = false;
                // packetRecvd = true;                
                numReceived = ndx;
                ndx = 0;
            }
        }
        else if (value == pkt_start)
        {
            recvInProgress = true;
        }        
    }
    decodePacket(_flowBuffer);
};

void Bronkhorst_FM::decodePacket(char* _flowBuffer)
{
    char raw_buff[2] = {0};
    int int_buff[5] = {0};
    int strol_result = 0;
    _fm_packet = {0};
    fm_value_set = {0};
    long combined_value = 0;


    // data length
    raw_buff[0] = _flowBuffer[0];
    raw_buff[1] = _flowBuffer[1];
    _fm_packet.length = (int)strtol(raw_buff, NULL, 16);

    // sender node
    raw_buff[0] = _flowBuffer[2];
    raw_buff[1] = _flowBuffer[3];
    _fm_packet.node = (int)strtol(raw_buff, NULL, 16);

    // command
    raw_buff[0] = _flowBuffer[4];
    raw_buff[1] = _flowBuffer[5];
    _fm_packet.command = (int)strtol(raw_buff, NULL, 16);

    // process
    raw_buff[0] = _flowBuffer[6];
    raw_buff[1] = _flowBuffer[7];
    _fm_packet.process = (int)strtol(raw_buff, NULL, 16);

    // data type ANDed with parameter (high byte: data type)
    raw_buff[0] = _flowBuffer[8];
    raw_buff[1] = _flowBuffer[9];
    _fm_packet.parameter = (int)strtol(raw_buff, NULL, 16);    

    if (_fm_packet.command == 0x00)
    {
        if (_fm_packet.process == 0x00) 
        {
            // signal write success
            // Serial.println("Write Success! ");
        }
    }    
    if (_fm_packet.process == 0x01 && _fm_packet.parameter == 0x04)
    {
        raw_buff[0] = _flowBuffer[10];
        raw_buff[1] = _flowBuffer[11];
        _fm_packet.int_val = (int)strtol(raw_buff, NULL, 16);

        fm_param._mode = _fm_packet.int_val;
    }
    if (_fm_packet.process == 0x01 && _fm_packet.parameter == 0x21) // Measure or Setpoint
    {
        raw_buff[0] = _flowBuffer[10];
        raw_buff[1] = _flowBuffer[11];
        _fm_packet.int_val = (int)strtol(raw_buff, NULL, 16);
        raw_buff[0] = _flowBuffer[12];
        raw_buff[1] = _flowBuffer[13];
        _fm_packet.int_val = (_fm_packet.int_val << 8) | (int)strtol(raw_buff, NULL, 16);
        
        if (fm_param.rd_stpnt_flg == 1)
        {
            fm_param._setpoint = _fm_packet.int_val;
            fm_param.rd_stpnt_flg = 0;
        }
        else
        {
            fm_param._measure = _fm_packet.int_val;            
        }
        
    }
    if (_fm_packet.process == 0x21 && _fm_packet.parameter == 0x40) // FMeasure
    {
        int j = 0;
        for (int i = 10; i <= ((_fm_packet.length * 2) + 2); i++)
        {
            raw_buff[0] = _flowBuffer[i];
            raw_buff[1] = _flowBuffer[(i + 1)];
            strol_result = (int)strtol(raw_buff, NULL, 16);
            i++;
            int_buff[j] = strol_result;
            j++;
        }
        fm_value_set.asINT[0] = (int_buff[2] << 8) | int_buff[3];
        fm_value_set.asINT[1] = (int_buff[0] << 8) | int_buff[1];

        _fm_packet.fl_val = fm_value_set.asFLOAT;
        
        fm_param._fmeasure = fm_value_set.asFLOAT;        
    }
    if (_fm_packet.process == 0x21 && _fm_packet.parameter == 0x47) // Temperature
    {
        int j = 0;
        for (int i = 10; i <= ((_fm_packet.length * 2) + 2); i++)
        {
            raw_buff[0] = _flowBuffer[i];
            raw_buff[1] = _flowBuffer[(i + 1)];
            strol_result = (int)strtol(raw_buff, NULL, 16);
            i++;
            int_buff[j] = strol_result;
            j++;
        }
        fm_value_set.asINT[0] = (int_buff[2] << 8) | int_buff[3];
        fm_value_set.asINT[1] = (int_buff[0] << 8) | int_buff[1];
        _fm_packet.fl_val = fm_value_set.asFLOAT;
        
        fm_param._temp = fm_value_set.asFLOAT;
    }
    if (_fm_packet.process == 0x71 && _fm_packet.parameter == 0x63)
    {
        int j = 0;
        for (int i = 11; i <= ((_fm_packet.length * 2) + 2); i++)
        {
            raw_buff[0] = _flowBuffer[i];
            raw_buff[1] = _flowBuffer[(i + 1)];
            strol_result = (int)strtol(raw_buff, NULL, 16);
            i++;
            str_buff[j++] = (char)strol_result;
        }
        str_buff[j + 1] = '\0';
        // Serial.println();
        // for (int i = 0; i < sizeof(str_buff); i++)
        // {
        //     Serial.print(str_buff[i]);
        // }
        // Serial.println();
    }
    if (_fm_packet.process == 0x74 && _fm_packet.parameter == 0x4F)
    {
        int j = 0;
          for (int i = 10; i <= ((_fm_packet.length * 2) + 2); i++)
          {
              raw_buff[0] = _flowBuffer[i];
              raw_buff[1] = _flowBuffer[(i + 1)];
              strol_result = (int)strtol(raw_buff, NULL, 16);
              i++;
              int_buff[j] = strol_result;
              j++;
          }
          fm_value_set.asINT[0] = (int_buff[2] << 8) | int_buff[3];
          fm_value_set.asINT[1] = (int_buff[0] << 8) | int_buff[1];
          _fm_packet.fl_val = fm_value_set.asFLOAT;
          
          fm_param._actDensity = fm_value_set.asFLOAT;
    }


    if (_fm_packet.process == 0x72 && _fm_packet.parameter == 0x41)
    {
        int j = 0;
        
        for (int i = 10; i <= ((_fm_packet.length * 2) + 2); i++)
        {
            raw_buff[0] = _flowBuffer[i];
            raw_buff[1] = _flowBuffer[i + 1];
            strol_result = (int)strtol(raw_buff, NULL, 16);
            i++;
            int_buff[j] = strol_result;
            j++;
        }

        // Combine 4 bytes into a single 32-bit integer
        combined_value = ((long)int_buff[0] << 24) | ((long)int_buff[1] << 16) | ((long)int_buff[2] << 8) | (long)int_buff[3];

        // Store the combined value into the appropriate packet and parameter fields
        _fm_packet.int32_val = combined_value;
        fm_param._valOutput = (float)combined_value;  // Assuming _actDensity should be a float representation of the long value
    }

    if (_fm_packet.process == 0x71 && _fm_packet.parameter == 0x63)
    {
        int j = 0;
        // Extract each pair of hex digits and convert to ASCII
        for (int i = 10; i < (10 + 20); i += 2) {  // 20 hex digits for 10 characters
            raw_buff[0] = _flowBuffer[i];
            raw_buff[1] = _flowBuffer[i + 1];
            raw_buff[2] = '\0';  // Null-terminate the string
            strol_result = (int)strtol(raw_buff, NULL, 16);
            fm_param._serialNumber[j++] = (char)strol_result;
        }
        fm_param._serialNumber[j] = '\0';  // Null-terminate the ASCII string

        // Debug output
        Serial.print("Serial number: ");
        Serial.println(fm_param._serialNumber);
    }

    
    // Debug output
    if (packetRecvd == true)
    {
        Serial.print("\n\rLength: ");
        Serial.println(_fm_packet.length, HEX);
        Serial.print("Node: ");
        Serial.println(_fm_packet.node, HEX);
        Serial.print("Command: ");
        Serial.println(_fm_packet.command, HEX);
        Serial.print("Process: ");
        Serial.println(_fm_packet.process, HEX);
        Serial.print("Parameter: ");
        Serial.println(_fm_packet.parameter, HEX);
        Serial.print("Int Value: ");
        Serial.println(_fm_packet.int_val, HEX);
        Serial.print("Int32 Value: ");
        Serial.println(_fm_packet.int32_val, HEX);
        Serial.print("Float Value: ");
        Serial.println(_fm_packet.fl_val);

        Serial.print("Packet:  ");
        for (byte n = 0; n < numReceived; n++) {
            Serial.print(_flowBuffer[n], HEX);
            Serial.print(' ');
        }

        Serial.println();
        packetRecvd = false;              
    }
    
};
