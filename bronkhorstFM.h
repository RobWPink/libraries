/**
 * @file bronkhorstFM.h
 * @brief Interface for the Bronkhorst Flow Meter
 * @author Christopher Long, ONEH2
 * @date May 19, 2022
 * @version 0.0
 */
#ifndef BRONKHORSTFM_H
#define BRONHORSTFM_H

#include <Arduino.h>

#define PACKET_SIZE 64

enum modes
{
  BUS_RS232 = 0x00,
  ANALOG_INPUT = 0x01,
  RS232 = 0x12,  
};

class Bronkhorst_FM
{
public:
  Bronkhorst_FM(Stream &s);

  int rd_controlmode(void);
  int rd_setpoint(void);
  int rd_measure(void);
  float rd_fmeasure(void);
  float rd_temp(void);

  int wr_controlmode(int mode);
  int wr_setpoint(int setpoint);

private:
  Stream *_s;
  struct bronkhorstFM_param_t
  {
    byte rd_stpnt_flg = 0;
    byte rd_meas_flg = 0;
    int _mode = 0;
    int _setpoint = 0;
    int _measure = 0;
    float _fmeasure = 0;
    float _temp = 0;
  };
  struct fm_packet_t
  {
    int length;
    int node;
    int command;
    int process;
    int parameter;
    int int_val;
    float fl_val;
    int32_t int32_val;
  };
  union f_m_value_set
  {
    uint16_t asINT[2];
    float asFLOAT;
  };  
  char _flowBuffer[PACKET_SIZE];

  bronkhorstFM_param_t fm_param;
  fm_packet_t _fm_packet;
 
  f_m_value_set fm_value_set;
  char serialnum[24];
  char str_buff[24];

  // only used in debug at the moment
  bool packetRecvd = false; 
  byte numReceived;

  void receiveFMPacket(void);
  void decodePacket(char* _flowbuffer);

};

#endif /*BRONKHORSTFM_H*/