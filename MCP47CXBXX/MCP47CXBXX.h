/**
 * @file MCP47CXBXX.h
 * @brief Arduino library for 12 bit I2C DAC - MCP47CxBxx
 * @author Christopher Long, ONEH2
 * @date July, 13 2022
 */

#ifndef _MCP47CXBXX_H_
#define _MCP47CXBXX_H_

#include <Arduino.h>
#include <Wire.h>

#define DAC_MAX_VAL 4095
#define DAC_RW_ERROR 0x00
#define DAC_RW_SUCCESS 0x01
#define DAC_VAL_ERROR 0xE1
#define DAC_REG_ERROR 0xE2
#define DAC_NOT_CONNECTED 0xEE

class MCP47CXBXX {
    public:
        MCP47CXBXX(uint8_t address, TwoWire* wire);

        void begin(void);
        bool isConnected(void);
        void disable(void);
        uint8_t wr_VOUT_0(uint16_t value);
        uint8_t wr_VOUT_1(uint16_t value);
        
    private:
        TwoWire* _wire;
        uint16_t _lastVal;
        uint8_t _address;

        uint8_t wr_Register(uint8_t reg, uint16_t value);
        uint8_t rd_Register(uint8_t reg, uint8_t* buffer);
};

#endif