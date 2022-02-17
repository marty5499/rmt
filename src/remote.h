#ifndef __MAIN__
#define __MAIN__
#include "Arduino.h"
#include "USB.h"
#include <WiFi.h>
#include <PubSubClient.h>
#include "ESP32TimerInterrupt.h"

#ifdef ESP32S2
// USB
USBCDC USBSerial;
#define Serial USBSerial
#endif

// WiFi
char msg[50];
WiFiClient espClient;
PubSubClient client(espClient);
const char *_ssid = "webduino.io";
const char *_password = "webduino";
const char *mqtt_server = "mqtt1.webduino.io";

// switch
bool sw = true;
bool refresh = false;

///// timer //////
ESP32Timer ITimer0(0);

void onMsg(String msg);

void callback(char *topic, byte *message, unsigned int length)
{
    String messageTemp;
    for (int i = 0; i < length; i++)
    {
        messageTemp += (char)message[i];
    }
    onMsg(messageTemp);
}

void startMQTT()
{
    client.setServer(mqtt_server, 1883);
    client.setCallback(callback);
    if (client.connect("mqttTest", "webduino", "webduino"))
    {
        Serial.println("mqtt connected");
        client.subscribe("test5499");
    }
}

void startWiFi()
{
    WiFi.setTxPower(WIFI_POWER_19_5dBm);
    WiFi.setAutoConnect(false);
    WiFi.disconnect(true, true);

    WiFi.setHostname("ESP32-Test");
    WiFi.begin(_ssid, _password);
    while ((WiFi.status() != WL_CONNECTED))
    {
        WiFi.begin(_ssid, _password);
        delay(3000);
        Serial.println("connecting...");
    }
    Serial.println("wifi connected");
}

void remote()
{
    startWiFi();
    startMQTT();
}

void timerTrigger();

void setup()
{
#ifdef ESP32S2
    USB.begin();
#endif
    Serial.begin(115200);
    setCpuFrequencyMhz(240);
    Serial.println("setup...");
    init();
}

void loop()
{
    client.loop();
}

void startTimer(unsigned long t)
{
    ITimer0.attachInterruptInterval(t, timerTrigger);
}

#endif