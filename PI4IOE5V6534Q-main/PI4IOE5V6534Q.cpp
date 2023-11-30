/**
 * @file PI4IOE5V6534Q.cpp
 * @brief Arduino library for 4 Port I2C IO Expander - PI4IOE5V6534Q
 * @author Christopher Long, ONEH2
 * @date July, 14 2022
 */
#include "PI4IOE5V6534Q.h"

PI4IOE5V6534Q::PI4IOE5V6534Q(uint8_t address, TwoWire& wire) :
    _wire(&wire),
    _address(address)
{
}

PI4IOE5V6534Q::PI4IOE5V6534Q(uint8_t address, TwoWire& wire, uint8_t irqPin, void (*irqFunc)()) :
    _wire(&wire),
    _address(address),
    _irqPin(irqPin),
    _irqFunc(irqFunc),
    _useIRQ(true)
{
}

void PI4IOE5V6534Q::begin()
{
    _wire->begin(); // Initialize I2C Bus
    wr_Register(IO_EXP_CONFIG_PORT0_REG, 0xFF);
    wr_Register(IO_EXP_CONFIG_PORT1_REG, 0xFF);
    wr_Register(IO_EXP_CONFIG_PORT2_REG, 0xFF);
    wr_Register(IO_EXP_CONFIG_PORT3_REG, 0xFF);
    wr_Register(IO_EXP_CONFIG_PORT4_REG, 0xFF);
}

bool PI4IOE5V6534Q::isConnected(void)
{
    _wire->beginTransmission(_address);
    if (_wire->endTransmission() == 0)
    {
        return true;
    }
    else
        return false;
}

uint8_t PI4IOE5V6534Q::pinMode(uint8_t pin, uint8_t mode, bool invert)
{
    uint8_t rtn = IO_EXP_RW_ERROR;    
    uint8_t _pin = 0x00;
    uint8_t _config_reg = 0x00;
    uint8_t _config_reg_val = 0xFF;
    uint8_t _config_reg_val_old = 0xFF;
    uint8_t _pud_en_reg = 0x00;
    uint8_t _pud_en_reg_val = 0x00;
    uint8_t _pud_en_reg_val_old = 0x00;
    uint8_t _pud_sel_reg = 0x00;
    uint8_t _pud_sel_reg_val = 0xFF;
    uint8_t _pud_sel_reg_val_old = 0xFF;
    uint8_t _pi_en_reg = 0x00;
    uint8_t _pi_en_reg_val = 0x00;
    uint8_t _pi_en_reg_val_old = 0x00;
    uint8_t _chk_val = 0x00;
    _invert = invert;
    #if defined(ENABLE_DEBUG_MESSAGES)
    Serial.print("\n\rInvert:  ");
    Serial.println(_invert);
    #endif

    if (pin < 0 || pin > 33)
    {
        return IO_EXP_PIN_ERROR;
    }
    if (pin >= 0 && pin <= 7)
    {
        _pin = pin;
        _config_reg = IO_EXP_CONFIG_PORT0_REG;
        _pud_en_reg = IO_EXP_PUPD_ENABLE_PORT0_REG;
        _pud_sel_reg = IO_EXP_PUPD_SELECT_PORT0_REG;
        if (_invert == true)
        {
            _pi_en_reg = IO_EXP_POL_INV_PORT0_REG;
        }        
        #if defined(ENABLE_DEBUG_MESSAGES)
        Serial.print("Set Mode Port 0 pin ");
        Serial.println(_pin);
        #endif
    }
    if (pin >= 8 && pin <= 15)
    {
        _pin = (pin - 8);
        _config_reg = IO_EXP_CONFIG_PORT1_REG;
        _pud_en_reg = IO_EXP_PUPD_ENABLE_PORT1_REG;
        _pud_sel_reg = IO_EXP_PUPD_SELECT_PORT1_REG;
        if (_invert == true)
        {
            _pi_en_reg = IO_EXP_POL_INV_PORT1_REG;
        }
        #if defined(ENABLE_DEBUG_MESSAGES)
        Serial.print("Set Mode Port 1 pin ");
        Serial.println(_pin);
        #endif
    }
    if (pin >= 16 && pin <= 23)
    {
        _pin = (pin - 16);
        _config_reg = IO_EXP_CONFIG_PORT2_REG;
        _pud_en_reg = IO_EXP_PUPD_ENABLE_PORT2_REG;
        _pud_sel_reg = IO_EXP_PUPD_SELECT_PORT2_REG;
        if (_invert == true)
        {
            _pi_en_reg = IO_EXP_POL_INV_PORT2_REG;
        }
        #if defined(ENABLE_DEBUG_MESSAGES)
        Serial.print("Set Mode Port 2 pin ");
        Serial.println(_pin);
        #endif
    }
    if (pin >= 24 && pin <= 31)
    {
        _pin = (pin - 24);
        _config_reg = IO_EXP_CONFIG_PORT3_REG;
        _pud_en_reg = IO_EXP_PUPD_ENABLE_PORT3_REG;
        _pud_sel_reg = IO_EXP_PUPD_SELECT_PORT3_REG;
        if (_invert == true)
        {
            _pi_en_reg = IO_EXP_POL_INV_PORT3_REG;
        }
        #if defined(ENABLE_DEBUG_MESSAGES)
        Serial.print("Set Mode Port 3 pin ");
        Serial.println(_pin);
        #endif
    }
    if (pin >= 32 && pin <= 33)
    {
        _pin = (pin - 32);
        _config_reg = IO_EXP_CONFIG_PORT4_REG;
        _pud_en_reg = IO_EXP_PUPD_ENABLE_PORT4_REG;
        _pud_sel_reg = IO_EXP_PUPD_SELECT_PORT4_REG;
        if (_invert == true)
        {
            _pi_en_reg = IO_EXP_POL_INV_PORT4_REG;
        }
        #if defined(ENABLE_DEBUG_MESSAGES)
        Serial.print("Set Mode Port 4 pin ");
        Serial.println(_pin);
        #endif
    }

    // Get the current register settings
    rd_Register(_config_reg, &_config_reg_val_old);
    rd_Register(_pud_en_reg, &_pud_en_reg_val_old);
    rd_Register(_pud_sel_reg, &_pud_sel_reg_val_old);
    if (_invert == true)
    {
        rd_Register(_pi_en_reg, &_pi_en_reg_val_old);
    }

    #if defined(ENABLE_DEBUG_MESSAGES)
    Serial.print("Config Register current val: ");
    Serial.println(_config_reg_val_old, HEX);

    Serial.print("Pullup/down Enable Register current val: ");
    Serial.println(_pud_en_reg_val_old, HEX);

    Serial.print("Pullup/down Select Register current val: ");
    Serial.println(_pud_sel_reg_val_old, HEX);
    if (_invert == true)
    {
        Serial.print("Polarity Inversion Register current val: ");
        Serial.println(_pi_en_reg_val_old, HEX);
    }
    #endif

    switch (mode)
    {
    case INPUT:
        _config_reg_val = (_config_reg_val_old |= 1 << _pin);
        wr_Register(_config_reg, _config_reg_val);
        rd_Register(_config_reg, &_chk_val);
        if(_chk_val == _config_reg_val)
        {
            #if defined(ENABLE_DEBUG_MESSAGES)
            Serial.println("Set to INPUT.");
            #endif
            rtn = IO_EXP_RW_SUCCESS;
        }

        if (_invert == true)
        {
            _pi_en_reg_val = (_pi_en_reg_val_old |= 1 << _pin);
            wr_Register(_pi_en_reg, _pi_en_reg_val);
            rd_Register(_pi_en_reg, &_chk_val);
            if (_chk_val == _pi_en_reg_val)
            {
                #if defined(ENABLE_DEBUG_MESSAGES)
                Serial.print("INPUT inverted: ");
                Serial.println(_chk_val, HEX);
                #endif
                _chk_val = 0;
                rtn = IO_EXP_RW_SUCCESS;
            }                        
        }              
        break;
    case OUTPUT:        
        _config_reg_val = _config_reg_val_old &= ~(1 << _pin);
        wr_Register(_config_reg, _config_reg_val);
        rd_Register(_config_reg, &_chk_val);
        if(_chk_val == _config_reg_val)
        {
            #if defined(ENABLE_DEBUG_MESSAGES)
            Serial.print("OUTPUT mode: ");
            Serial.println(_chk_val, HEX);
            #endif
            rtn = IO_EXP_RW_SUCCESS;
        }                
        break;
    case INPUT_PULLUP:
        _config_reg_val = (_config_reg_val_old |= 1 << _pin);
        wr_Register(_config_reg, _config_reg_val);
        rd_Register(_config_reg, &_chk_val);
        if(_chk_val == _config_reg_val)
        {
            #if defined(ENABLE_DEBUG_MESSAGES)
            Serial.print("INPUT mode: ");
            Serial.println(_chk_val, HEX);
            #endif
            _chk_val = 0;
            rtn = IO_EXP_RW_SUCCESS;
        }

        _pud_en_reg_val = (_pud_en_reg_val_old |= 1 << _pin);        
        wr_Register(_pud_en_reg, _pud_en_reg_val);
        rd_Register(_pud_en_reg, &_chk_val);
        if(_chk_val == _pud_en_reg_val)
        {
            #if defined(ENABLE_DEBUG_MESSAGES)
            Serial.print("INPUT_PULLUP enabled: ");
            Serial.println(_chk_val, HEX);
            #endif
            _chk_val = 0;
            rtn = IO_EXP_RW_SUCCESS;
        }

        _pud_sel_reg_val =( _pud_sel_reg_val_old |= 1 << _pin);
        wr_Register(_pud_sel_reg, _pud_sel_reg_val);
        rd_Register(_pud_sel_reg, &_chk_val);
        if(_chk_val == _pud_sel_reg_val)
        {
            #if defined(ENABLE_DEBUG_MESSAGES)
            Serial.print("INPUT_PULLUP selected: ");
            Serial.println(_chk_val, HEX);
            #endif
            _chk_val = 0;
            rtn = IO_EXP_RW_SUCCESS;
        }

        if (_invert == true)
        {
            _pi_en_reg_val =( (_pi_en_reg_val_old |= 1 << _pin));
            wr_Register(_pi_en_reg, _pi_en_reg_val);
            rd_Register(_pi_en_reg, &_chk_val);
            if (_chk_val == _pi_en_reg_val)
            {
                #if defined(ENABLE_DEBUG_MESSAGES)
                Serial.print("INPUT inverted: ");
                Serial.println(_chk_val, HEX);
                #endif
                _chk_val = 0;
                rtn = IO_EXP_RW_SUCCESS;
            }                        
        }               
        break;
    case INPUT_PULLDOWN:
        _config_reg_val = _config_reg_val_old |= 1 << _pin;
        wr_Register(_config_reg, _config_reg_val);
        rd_Register(_config_reg, &_chk_val);
        if(_chk_val == _config_reg_val)
        {
            #if defined(ENABLE_DEBUG_MESSAGES)
            Serial.print("INPUT mode: ");
            Serial.println(_chk_val, HEX);
            #endif
            _chk_val = 0;
            rtn = IO_EXP_RW_SUCCESS;
        }

        _pud_en_reg_val = _pud_en_reg_val_old |= 1 << _pin;        
        wr_Register(_pud_en_reg, _pud_en_reg_val);
        rd_Register(_pud_en_reg, &_chk_val);
        if(_chk_val == _pud_en_reg_val)
        {
            #if defined(ENABLE_DEBUG_MESSAGES)
            Serial.print("INPUT_PULLDOWN enabled: ");
            Serial.println(_chk_val, HEX);
            #endif
            _chk_val = 0;
            rtn = IO_EXP_RW_SUCCESS;
        }

        _pud_sel_reg_val = (_pud_sel_reg_val &= ~(1 << _pin));
        wr_Register(_pud_sel_reg, _pud_sel_reg_val);
        rd_Register(_pud_sel_reg, &_chk_val);
        if(_chk_val == _pud_sel_reg_val)
        {
            #if defined(ENABLE_DEBUG_MESSAGES)
            Serial.print("INPUT_PULLDOWN selected: ");
            Serial.println(_chk_val, HEX);
            #endif
            _chk_val = 0;
            rtn = IO_EXP_RW_SUCCESS;
        }

        if (_invert == true)
        {
            _pi_en_reg_val = (_pi_en_reg_val_old |= 1 << _pin);
            wr_Register(_pi_en_reg, _pi_en_reg_val);
            rd_Register(_pi_en_reg, &_chk_val);
            if (_chk_val == _pi_en_reg_val)
            {
                #if defined(ENABLE_DEBUG_MESSAGES)
                Serial.print("INPUT inverted: ");
                Serial.println(_chk_val, HEX);
                #endif
                _chk_val = 0;
                rtn = IO_EXP_RW_SUCCESS;
            }                        
        }      
        break;
    default:
        rtn = IO_EXP_MODE_ERROR;
        break;
    }

    return rtn;
}

uint8_t PI4IOE5V6534Q::digitalRead(uint8_t pin, uint8_t *rtn_val)
{
    uint8_t rtn = IO_EXP_RW_ERROR;
    uint8_t _pin = 0x0F;
    uint8_t _input_reg = 0xFF;
    uint8_t _reg_val = 0x00;

    if (pin < 0 || pin > 33)
        return IO_EXP_PIN_ERROR;

    if (pin >= 0 && pin <= 7)
    {
        _pin = pin;
        _input_reg = IO_EXP_INPUT_PORT0_REG;
        #if defined(ENABLE_DEBUG_MESSAGES)
        Serial.print("\n\rRead Port 0 pin ");
        Serial.println(_pin);
        #endif

    }
    if (pin >= 8 && pin <= 15)
    {
        _pin = (pin - 8);
        _input_reg = IO_EXP_INPUT_PORT1_REG;
        #if defined(ENABLE_DEBUG_MESSAGES)
        Serial.print("\n\rRead Port 1 pin ");
        Serial.println(_pin);
        #endif

    }
    if (pin >= 16 && pin <= 23)
    {
        _pin = (pin - 16);
        _input_reg = IO_EXP_INPUT_PORT2_REG;
        #if defined(ENABLE_DEBUG_MESSAGES)
        Serial.print("\n\rRead Port 2 pin ");
        Serial.println(_pin);
        #endif

    }
    if (pin >= 24 && pin <= 31)
    {
        _pin = (pin - 24);
        _input_reg = IO_EXP_INPUT_PORT3_REG;
        #if defined(ENABLE_DEBUG_MESSAGES)
        Serial.print("\n\rRead Port 3 pin ");
        Serial.println(_pin);
        #endif

    }
    if (pin >= 32 && pin <= 33)
    {
        _pin = (pin - 32);
        _input_reg = IO_EXP_INPUT_PORT4_REG;
        #if defined(ENABLE_DEBUG_MESSAGES)
        Serial.print("\n\rRead Port 4 pin ");
        Serial.println(_pin);
        #endif

    }
    
    rd_Register(_input_reg, &_reg_val);
    _reg_val = (_reg_val >> _pin) & 1;
    *rtn_val = _reg_val;

    return rtn;
}

uint8_t PI4IOE5V6534Q::digitalWrite(uint8_t pin, uint8_t val)
{
    uint8_t rtn = IO_EXP_RW_ERROR;
    uint8_t _pin = 0x00;
    uint8_t _output_reg = 0x00;
    uint8_t _reg_val = 0x00;
    uint8_t _reg_val_old = 0x00;
    uint8_t _chk_val = 0x00;

    if (pin < 0 || pin > 33)
        return IO_EXP_PIN_ERROR;

    if (pin >= 0 && pin <= 7)
    {
        _pin = pin;
        _output_reg = IO_EXP_OUTPUT_PORT0_REG;
        #if defined(ENABLE_DEBUG_MESSAGES)        
        Serial.print("\n\rWrite Port 0 pin ");
        Serial.println(_pin);
        #endif
    }
    if (pin >= 8 && pin <= 15)
    {
        _pin = (pin - 8);
        _output_reg = IO_EXP_OUTPUT_PORT1_REG;
        #if defined(ENABLE_DEBUG_MESSAGES)
        Serial.print("\n\rWrite Port 1 pin ");
        Serial.println(_pin);
        #endif
    }
    if (pin >= 16 && pin <= 23)
    {
        _pin = (pin - 16);
        _output_reg = IO_EXP_OUTPUT_PORT2_REG;
        #if defined(ENABLE_DEBUG_MESSAGES)
        Serial.print("\n\rWrite Port 2 pin ");
        Serial.println(_pin);
        #endif
    }
    if (pin >= 24 && pin <= 31)
    {
        _pin = (pin - 24);
        _output_reg = IO_EXP_OUTPUT_PORT3_REG;
        #if defined(ENABLE_DEBUG_MESSAGES)
        Serial.print("\n\rWrite Port 3 pin ");
        Serial.println(_pin);
        #endif
    }
    if (pin >= 32 && pin <= 33)
    {
        _pin = (pin - 32);
        _output_reg = IO_EXP_OUTPUT_PORT4_REG;
        #if defined(ENABLE_DEBUG_MESSAGES)
        Serial.print("\n\rWrite Port 4 pin ");
        Serial.println(_pin);
        #endif
    }

    // Get the current register settings
    rd_Register(_output_reg, &_reg_val_old);    

    switch (val)
    {
    case HIGH:
        _reg_val = (_reg_val_old |= 1 << _pin);
        wr_Register(_output_reg, _reg_val);
        rd_Register(_output_reg, &_chk_val);
        if(_chk_val == _reg_val)
        {
            #if defined(ENABLE_DEBUG_MESSAGES)
            Serial.println("Pin set HIGH.");
            #endif
            rtn = IO_EXP_RW_SUCCESS;
        }              
        break;
    case LOW:
        _reg_val = (_reg_val_old &= ~(1 << _pin));
        wr_Register(_output_reg, _reg_val);
        rd_Register(_output_reg, &_chk_val);
        if(_chk_val == _reg_val)
        {
            #if defined(ENABLE_DEBUG_MESSAGES)
            Serial.println("Pin set LOW.");
            #endif
            rtn = IO_EXP_RW_SUCCESS;
        }                
        break;
    default:
        rtn = IO_EXP_VAL_ERROR;
        break;
    }

    return rtn;
}

uint8_t PI4IOE5V6534Q::portMode(uint8_t port, uint8_t mode)
{
    uint8_t rtn = IO_EXP_RW_ERROR;
    uint8_t _config_reg = 0x00;
    uint8_t _reg_val = 0x00;
    uint8_t _chk_val = 0x00;
    uint8_t _pud_en_reg = 0x00;
    uint8_t _pud_en_reg_val = 0x00;
    uint8_t _pud_sel_reg = 0x00;
    uint8_t _pud_sel_reg_val = 0x00;
    uint8_t _pi_en_reg = 0x00;
    uint8_t _pi_en_reg_val = 0x00;

    switch (port)
    {
    case IO_EXP_PORT0:
        _config_reg = IO_EXP_CONFIG_PORT0_REG;
        _pud_en_reg = IO_EXP_PUPD_ENABLE_PORT0_REG;
        _pud_sel_reg = IO_EXP_PUPD_SELECT_PORT0_REG;
        break;
    case IO_EXP_PORT1:
        _config_reg = IO_EXP_CONFIG_PORT1_REG;
        _pud_en_reg = IO_EXP_PUPD_ENABLE_PORT1_REG;
        _pud_sel_reg = IO_EXP_PUPD_SELECT_PORT1_REG;
        break;
    case IO_EXP_PORT2:
        _config_reg = IO_EXP_CONFIG_PORT2_REG;
        _pud_en_reg = IO_EXP_PUPD_ENABLE_PORT2_REG;
        _pud_sel_reg = IO_EXP_PUPD_SELECT_PORT2_REG;
        break;
    case IO_EXP_PORT3:
        _config_reg = IO_EXP_CONFIG_PORT3_REG;
        _pud_en_reg = IO_EXP_PUPD_ENABLE_PORT3_REG;
        _pud_sel_reg = IO_EXP_PUPD_SELECT_PORT3_REG;
        break;
    case IO_EXP_PORT4:
        _config_reg = IO_EXP_CONFIG_PORT4_REG;
        _pud_en_reg = IO_EXP_PUPD_ENABLE_PORT4_REG;
        _pud_sel_reg = IO_EXP_PUPD_SELECT_PORT4_REG;
        break;
    default:
        break;
    }

    switch (mode)
    {
    case INPUT:
        _reg_val = 0xFF;
        wr_Register(_config_reg, _reg_val);
        rd_Register(_config_reg, &_chk_val);
        if(_chk_val == _reg_val)
        {
            rtn = IO_EXP_RW_SUCCESS;
        }        
        #if defined(ENABLE_DEBUG_MESSAGES)
        Serial.println("INPUT mode\n\r");
        #endif
        break;
    case OUTPUT:
        _reg_val = 0x00;
        wr_Register(_config_reg, _reg_val);
        rd_Register(_config_reg, &_chk_val);
        if(_chk_val == _reg_val)
        {
            rtn = IO_EXP_RW_SUCCESS;
        }
        #if defined(ENABLE_DEBUG_MESSAGES)
        Serial.println("OUTPUT mode\n\r");
        #endif
        break;
    case INPUT_PULLUP:
        _reg_val = 0xFF;
        wr_Register(_config_reg, _reg_val);
        wr_Register(_pud_en_reg, _pud_en_reg_val);
        wr_Register(_pud_sel_reg, _pud_sel_reg_val);
        wr_Register(_pi_en_reg, _pi_en_reg_val);
        rd_Register(_config_reg, &_chk_val);
        if(_chk_val == _reg_val)
        {
            rtn = IO_EXP_RW_SUCCESS;
        }
        #if defined(ENABLE_DEBUG_MESSAGES)
        Serial.println("INPUT_PULLUP mode\n\r");
        #endif
        break;
    case INPUT_PULLDOWN:
        _reg_val = 0xFF;
        wr_Register(_config_reg, _reg_val);
        wr_Register(_pud_en_reg, _pud_en_reg_val);
        wr_Register(_pud_sel_reg, _pud_sel_reg_val);
        rtn = IO_EXP_RW_SUCCESS;
        #if defined(ENABLE_DEBUG_MESSAGES)
        Serial.println("INPUT_PULLDOWN mode\n\r");
        #endif
        break;
    default:
        break;
    }
        
    return rtn;
}

uint8_t PI4IOE5V6534Q::readPort(uint8_t port, uint8_t *rtn_val)
{
    uint8_t rtn = IO_EXP_RW_ERROR;
    uint8_t _input_reg = 0x00;
    uint8_t reg_val = 0x00;

    switch (port)
    {
    case IO_EXP_PORT0:
        _input_reg = IO_EXP_INPUT_PORT0_REG;
        break;
    case IO_EXP_PORT1:
        _input_reg = IO_EXP_INPUT_PORT1_REG;        
        break;
    case IO_EXP_PORT2:
        _input_reg = IO_EXP_INPUT_PORT2_REG;        
        break;
    case IO_EXP_PORT3:
        _input_reg = IO_EXP_INPUT_PORT3_REG;        
        break;
    case IO_EXP_PORT4:
        _input_reg = IO_EXP_INPUT_PORT4_REG;
        break;
    default:
        return IO_EXP_REG_ERROR;
        break;
    }

    if (rd_Register(_input_reg, &reg_val))
    {
        // reg_val = 0xB8; //test value
        *rtn_val = reg_val;
        rtn = IO_EXP_RW_SUCCESS;
    }
    else
    {
        Serial.println("IO Read Port failed.");
    }

    return rtn;
}

uint8_t PI4IOE5V6534Q::writePort(uint8_t port, uint8_t port_val)
{
    uint8_t rtn = IO_EXP_RW_ERROR;
    uint8_t reg_val = 0xB8;
    uint8_t _config_reg = 0x00;

    switch (port)
    {
    case IO_EXP_PORT0:
        _config_reg = IO_EXP_OUTPUT_PORT0_REG;
        break;
    case IO_EXP_PORT1:
        _config_reg = IO_EXP_OUTPUT_PORT1_REG;
        break;
    case IO_EXP_PORT2:
        _config_reg = IO_EXP_OUTPUT_PORT2_REG;
        break;
    case IO_EXP_PORT3:
        _config_reg = IO_EXP_OUTPUT_PORT3_REG;
        break;
    case IO_EXP_PORT4:
        _config_reg = IO_EXP_OUTPUT_PORT4_REG;
        break;
    default:
        return IO_EXP_REG_ERROR;;
        break;
    }
    if (!wr_Register(_config_reg, port_val))
    {
        #if defined(ENABLE_DEBUG_MESSAGES)
        Serial.println("IO Write Port failed.");
        #endif
    }
    if (!rd_Register(_config_reg, &reg_val))
    {
        #if defined(ENABLE_DEBUG_MESSAGES)
        Serial.println("IO Read Port failed.");
        #endif
    }
    else if (port_val == reg_val)
    {
        #if defined(ENABLE_DEBUG_MESSAGES)
        Serial.print(" Port val: ");
        Serial.println(reg_val);
        #endif
        rtn = IO_EXP_RW_SUCCESS;
    }
    
    return rtn;
}

uint8_t PI4IOE5V6534Q::rd_Register(uint8_t reg, uint8_t *buffer)
{
    uint8_t _register_val = 0x00;
    uint8_t _reg = reg;
    
    //Empty read to advance io internal pointer
    _wire->beginTransmission(_address);
    _wire->write(_reg);
    _wire->requestFrom(_address, 1);
    _register_val = _wire->read();
    _wire->endTransmission();

    // #if defined(ENABLE_DEBUG_MESSAGES)
    // Serial.print("1st Register read: ");
    // Serial.print(_reg, HEX);
    // Serial.print(" Value: ");
    // Serial.println(_register_val, HEX);
    // #endif

    //Valid read
    _wire->beginTransmission(_address);
    _wire->write(_reg);
    _wire->requestFrom(_address, 1);
    _register_val = _wire->read();    
    if (_wire->endTransmission() != 0)
    {        
        return 0;
    }

    *buffer = _register_val;

    // #if defined(ENABLE_DEBUG_MESSAGES)
    // Serial.print("Register read: ");
    // Serial.print(_reg, HEX);
    // Serial.print(" Value: ");
    // Serial.println(_register_val, HEX);
    // #endif

    return 1;
}

uint8_t PI4IOE5V6534Q::wr_Register(uint8_t reg, uint8_t value)
{
    _wire->beginTransmission(_address);
    _wire->write(reg);
    _wire->write(value);
     if (_wire->endTransmission() != 0)
    {
        return 0;
    }
    return 1;
}

void PI4IOE5V6534Q::report_status(ResultCode rtn)
{
  if(rtn != IO_EXP_RW_SUCCESS)
      switch (rtn)
      {
        case IO_EXP_RW_ERROR:
            #if defined(ENABLE_DEBUG_MESSAGES)
            Serial.println("IO_EXP_RW_ERROR");
            #endif
            break;
        case IO_EXP_VAL_ERROR:
            #if defined(ENABLE_DEBUG_MESSAGES)
            Serial.println("IO_EXP_REG_ERROR");
            #endif
            break;
        case  IO_EXP_PIN_ERROR:
            #if defined(ENABLE_DEBUG_MESSAGES)
            Serial.println("IO_EXP_PIN_ERROR");
            #endif
            break;
        case  IO_EXP_MODE_ERROR:
            #if defined(ENABLE_DEBUG_MESSAGES)
            Serial.println("IO_EXP_MODE_ERROR");
            #endif
            break;  
        case  IO_EXP_NOT_CONNECTED:
            #if defined(ENABLE_DEBUG_MESSAGES)
            Serial.println("IO_EXP_NOT_CONNECTED");
            #endif
            break;
        default:
            break;
      }
}