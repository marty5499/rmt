#ifndef __MAIN__
#define __MAIN__
#include "Arduino.h"
#include "USB.h"
#include "ESP32TimerInterrupt.h"
#define TIMER0_INTERVAL_MS 1000

// USB
extern USBCDC USBSerial;
#define Serial USBSerial
#endif