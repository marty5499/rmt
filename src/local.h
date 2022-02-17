#ifndef __MAIN__
#define __MAIN__
#include "Arduino.h"

#ifdef ESP32S2
#include "USB.h"
// USB
USBCDC USBSerial;
#define Serial USBSerial
#endif
extern void setting();

void setup()
{
#ifdef ESP32S2
    USB.begin();
#endif
    Serial.begin(115200);
    // setCpuFrequencyMhz(240);
    Serial.println("setup...");
    setting();
}

void loop()
{
}

#endif