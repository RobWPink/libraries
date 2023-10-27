/**
 * @file PI4IOE5V6534Q.h
 * @brief Arduino library for 4 Port I2C IO Expander - PI4OIE5V6534Q
 * @author Christopher Long, ONEH2
 * @date July, 14 2022
 */
#ifndef _PI4IOE5V6534Q_H_
#define _PI4IOE5V6534Q_H_

#include <Arduino.h>
#include <Wire.h>

// #define ENABLE_DEBUG_MESSAGES

#define INPUT_PULLDOWN 0x03
#define MAX_IO_EXP_PINS 34

#define IO_EXP_PORT_MODE_INPUT 0xFF
#define IO_EXP_PORT_MODE_OUTPUT 0x00

#define IO_EXP_PORT0 0x00 
#define IO_EXP_PORT1 0x01
#define IO_EXP_PORT2 0x02
#define IO_EXP_PORT3 0x03
#define IO_EXP_PORT4 0x04

enum Pins {
    P0_0, P0_1, P0_2, P0_3, P0_4, P0_5, P0_6, P0_7,
    P1_0, P1_1, P1_2, P1_3, P1_4, P1_5, P1_6, P1_7,
    P2_0, P2_1, P2_2, P2_3, P2_4, P2_5, P2_6, P2_7,
    P3_0, P3_1, P3_2, P3_3, P3_4, P3_5, P3_6, P3_7,
    P4_0, P4_1
};

class PI4IOE5V6534Q {
    public:
        PI4IOE5V6534Q(uint8_t address, TwoWire& wire);
        PI4IOE5V6534Q(uint8_t address, TwoWire& wire, uint8_t irqPin, void (*irqFunc)());
        
        void begin();
        bool isConnected(void);
        uint8_t pinMode(uint8_t pin, uint8_t mode, bool invert = false);
        uint8_t digitalRead(uint8_t pin, uint8_t* rtn_val);
        uint8_t digitalWrite(uint8_t pin, uint8_t val);
        uint8_t portMode(uint8_t port, uint8_t mode);
        uint8_t readPort(uint8_t port, uint8_t* rtn_val);
        uint8_t writePort(uint8_t port, uint8_t port_val);

        enum ResultCode {
            IO_EXP_RW_ERROR = 0x00,
            IO_EXP_RW_SUCCESS = 0x01,
            IO_EXP_VAL_ERROR = 0xE1,
            IO_EXP_REG_ERROR = 0xE2,
            IO_EXP_PIN_ERROR = 0xE3,
            IO_EXP_MODE_ERROR = 0xE4,
            IO_EXP_NOT_CONNECTED = 0xEE
        };
        enum RegisterAddresses {
            IO_EXP_INPUT_PORT0_REG = 0x00,
            IO_EXP_INPUT_PORT1_REG = 0x01,
            IO_EXP_INPUT_PORT2_REG = 0x02,
            IO_EXP_INPUT_PORT3_REG = 0x03,
            IO_EXP_INPUT_PORT4_REG = 0x04,

            IO_EXP_OUTPUT_PORT0_REG = 0x05,
            IO_EXP_OUTPUT_PORT1_REG = 0x06,
            IO_EXP_OUTPUT_PORT2_REG = 0x07,
            IO_EXP_OUTPUT_PORT3_REG = 0x08,
            IO_EXP_OUTPUT_PORT4_REG = 0x09,

            IO_EXP_POL_INV_PORT0_REG = 0x0A,
            IO_EXP_POL_INV_PORT1_REG = 0x0B,
            IO_EXP_POL_INV_PORT2_REG = 0x0C,
            IO_EXP_POL_INV_PORT3_REG = 0x0D,
            IO_EXP_POL_INV_PORT4_REG = 0x0E,

            IO_EXP_CONFIG_PORT0_REG = 0x0F,
            IO_EXP_CONFIG_PORT1_REG = 0x10,
            IO_EXP_CONFIG_PORT2_REG = 0x11,
            IO_EXP_CONFIG_PORT3_REG = 0x12,
            IO_EXP_CONFIG_PORT4_REG = 0x13,

            IO_EXP_INPUT_LAT_PORT0_REG = 0x3A,
            IO_EXP_INPUT_LAT_PORT1_REG = 0x3B,
            IO_EXP_INPUT_LAT_PORT2_REG = 0x3C,
            IO_EXP_INPUT_LAT_PORT3_REG = 0x3D,
            IO_EXP_INPUT_LAT_PORT4_REG = 0x3E,

            IO_EXP_PUPD_ENABLE_PORT0_REG = 0x3F,
            IO_EXP_PUPD_ENABLE_PORT1_REG = 0x40,
            IO_EXP_PUPD_ENABLE_PORT2_REG = 0x41,
            IO_EXP_PUPD_ENABLE_PORT3_REG = 0x42,
            IO_EXP_PUPD_ENABLE_PORT4_REG = 0x43,

            IO_EXP_PUPD_SELECT_PORT0_REG = 0x44,
            IO_EXP_PUPD_SELECT_PORT1_REG = 0x45,
            IO_EXP_PUPD_SELECT_PORT2_REG = 0x46,
            IO_EXP_PUPD_SELECT_PORT3_REG = 0x47,
            IO_EXP_PUPD_SELECT_PORT4_REG = 0x48,

            IO_EXP_OUTPUT_PORT_CONFIG_REG = 0x53,

            IO_EXP_INPUT_STATUS_PORT0_REG = 0x63,
            IO_EXP_INPUT_STATUS_PORT1_REG = 0x64,
            IO_EXP_INPUT_STATUS_PORT2_REG = 0x65,
            IO_EXP_INPUT_STATUS_PORT3_REG = 0x66,
            IO_EXP_INPUT_STATUS_PORT4_REG = 0x67,

            IO_EXP_IOCR_PORT0_REG = 0x68,
            IO_EXP_IOCR_PORT1_REG = 0x69,
            IO_EXP_IOCR_PORT2_REG = 0x6A,
            IO_EXP_IOCR_PORT3_REG = 0x6B,
            IO_EXP_IOCR_PORT4_REG = 0x6C,            
        };
        
    private:
        TwoWire* _wire;
        uint8_t _address;

        bool _useIRQ = false;
        void (*_irqFunc)(){};
        uint8_t _irqPin;

        bool _invert = 0;

        uint8_t rd_Register(uint8_t reg, uint8_t* buffer);
        uint8_t wr_Register(uint8_t reg, uint8_t value);
        void report_status(ResultCode rtn);
        
        
};

#endif