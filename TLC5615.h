/*
 TLC5615.h
 TLC5615 driver for Arduino
 
 Copyright 2015-2016 Max Chan <max@maxchan.info>
 */

#ifndef _TLC5615_H_
#define _TLC5615_H_

#include <Arduino.h>

/// Driver for TI TLC5615 DAC.
class TLC5615
{
private:
    uint8_t _ss;
    
public:
    /*!
     Initialize the TLC5615 driver.
     
     @param ss GPIO pin used for the chip select for the DAC.
     */
    TLC5615(uint8_t ss);
    
    /*!
     Initialize the SPI bus and the DAC chip.
     */
    void begin(void);
    
    /*!
     Write a value to the DAC chip.
     
     @param value The DAC value. It is capped to 10 bits and excessive bits are
     ignored.
     */
    void dacanalogWrite(uint16_t value);
};

#endif // !defined(_TLC5615_H_)
