/**
 * @file MCP47CXBXX.cpp
 * @brief Arduino library for 12 bit I2C DAC - MCP47CxBxx
 * @author Christopher Long, ONEH2
 * @date July, 13 2022
 */

#include "MCP47CXBXX.h"

#define VOL_DAC0_REG 0x00
#define VOL_DAC1_REG 0x01
#define VOL_VREF_REG 0x08
#define VOL_POWDWN_REG 0x09
#define VOL_GAIN_STAT_REG 0x0A

MCP47CXBXX::MCP47CXBXX(uint8_t address, TwoWire *wire = &Wire)
{
    _wire = wire;
    _address = address;
}

void MCP47CXBXX::begin(void)
{
    _wire->begin(); // Initialize I2C Bus
    disable();      // Make sure outputs are powered off
}

bool MCP47CXBXX::isConnected(void)
{
    _wire->beginTransmission(_address);
    if(_wire->endTransmission() == 0)
    {
        return true;
    }
    else return false;
}

void MCP47CXBXX::disable(void)
{
    wr_VOUT_0(0x0000);
}

uint8_t MCP47CXBXX::wr_VOUT_0(uint16_t value)
{
    uint8_t rtn = DAC_RW_ERROR;
    uint8_t rd_reg_buff[2] = {0};

    if(value > DAC_MAX_VAL) return DAC_VAL_ERROR; 
    if(!isConnected()) return DAC_NOT_CONNECTED;

    rtn = wr_Register(VOL_DAC0_REG, value);
    if(rtn == DAC_RW_SUCCESS)
    {
        // Serial.println("Reading REG...");
        if(rd_Register(VOL_DAC0_REG, rd_reg_buff))
        {
            // Serial.println("REG read returned success.");
            // Serial.println(rd_reg_buff[0], HEX);
            // Serial.println(rd_reg_buff[1], HEX);
            _lastVal = (rd_reg_buff[0] << 8) | (rd_reg_buff[1]);
            if (_lastVal != value)
            {
                // Serial.println("\n\rREG value mismatch.");
                // Serial.print("_lastVal: ");
                // Serial.print(_lastVal);
                // Serial.print(" value: ");
                // Serial.println(value);
                return DAC_REG_ERROR;
            }
            else return DAC_RW_SUCCESS;
        }        
        else 
        {
            // Serial.println("REG read failed.");
            return DAC_REG_ERROR;
        }
    }
    else return rtn;
    
}

uint8_t MCP47CXBXX::wr_Register(uint8_t reg, uint16_t value)
{
    uint8_t l_byte = value & 0xFF;
    uint8_t h_byte = ((value >> 8) & 0x0F);

    _wire->beginTransmission(_address);
    _wire->write(reg);
    _wire->write(h_byte);
    _wire->write(l_byte);

    if (_wire->endTransmission() == 0)
    {
        return DAC_RW_SUCCESS;
    }
    else return DAC_RW_ERROR;    
}

uint8_t MCP47CXBXX::rd_Register(uint8_t reg, uint8_t* buffer)
{
    uint8_t count = 0;
    uint8_t rd_Bytes = 0;
    
    _wire->beginTransmission(_address);
    _wire->write(reg);
    rd_Bytes = _wire->requestFrom(_address, sizeof(buffer));    
    while (count < rd_Bytes)
    {
        buffer[count++] = _wire->read();
    }

    if (_wire->endTransmission() == 0)
    {
        return DAC_RW_SUCCESS;
    }
    else return DAC_RW_ERROR;      
}

