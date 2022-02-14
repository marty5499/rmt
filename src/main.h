#ifndef __MAIN__
#define __MAIN__
#include "Arduino.h"
#include "USB.h"

// USB
extern USBCDC USBSerial;
#define Serial USBSerial
#endif