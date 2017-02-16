/*
 TLC5615.cpp
 TLC5615 driver for Arduino
 
 Copyright 2015-2016 Max Chan <max@maxchan.info>
 */

#include "TLC5615.h"
#include <SPI.h>

TLC5615::TLC5615(uint8_t ss)
{
    _ss = ss;
}

void TLC5615::begin(void)
{
    // Prepare SPI hardware.
    SPI.begin();
    
    // Prepare SPI SS line
    digitalWrite(_ss, 1);
    pinMode(_ss, OUTPUT);
}

void TLC5615::dacanalogWrite(uint16_t value)
{
    // Prepare the buffer
    value = (value & 0x3ff) << 2;
    
    // Assert SPI bus
    digitalWrite(_ss, 0);
    
    // Perform SPI Transfer
    SPI.beginTransaction(SPISettings(20000000, MSBFIRST, SPI_MODE0));
    SPI.transfer16(value);
    SPI.endTransaction();
    
    // Deassert SPI bus
    digitalWrite(_ss, 1);
}

