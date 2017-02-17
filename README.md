# LibAPRS_ESP
Fork of LibAPRS for the ESP8266

The example is set up as a simple KISS TNC.  The example also sends a message packet every 150 secs using simpletimer library.  




This library uses external SPI ADC and DAC
ADC MCP3008
DAC TLC5615

Pinout:

D8 - CS MCP3008 (ADC)

D7 - MOSI

D6 - MISO

D5 - SCK

D4 - PTT To 2N7002

D3 - CS TLC5615 (DAC)

