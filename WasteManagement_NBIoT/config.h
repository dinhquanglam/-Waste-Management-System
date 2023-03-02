#include <SoftwareSerial.h>

#define rxPin 13 // tx trong module NB-IoT
#define txPin 12 // rx trong module NB-IoT
#define atRebootPin 11

SoftwareSerial serialPort(rxPin, txPin);

#define isHwReset 0
#define isATReset 1
#define isNetLight 0


//Serial configuration
#define baudrate 9600
