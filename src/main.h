#include "Arduino.h"
#include "USB.h"
#include "ESP32TimerInterrupt.h"
#define TIMER0_INTERVAL_MS 1000
USBCDC USBSerial;
ESP32Timer ITimer0(0);

