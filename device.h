

#ifndef DEVICE_CONFIGURATION
#define DEVICE_CONFIGURATION

// CPU settings
    #define F_CPU 80000000

#ifndef FREQUENCY_CORRECTION
    #define FREQUENCY_CORRECTION 0
#endif
extern int adcVal;  //Share adcVal with sketch for future


// Sampling & timer setup
#define CONFIG_AFSK_DAC_SAMPLERATE 9600


#endif