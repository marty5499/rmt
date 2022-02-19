#ifndef __MAIN__
#define __MAIN__
#include "Arduino.h"
#include "USB.h"
#include <WiFi.h>
#include <PubSubClient.h>

// USB
#ifdef ESP32S2
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
bool openWiFi = false;

extern void onMsg(String msg);
extern void appLoop();
extern void setting();

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
    WiFi.setAutoConnect(true);
    WiFi.disconnect(true, true);
    int i = 0;
    WiFi.setHostname("wa");
    Serial.print("ssid:" + String(_ssid));
    Serial.println(" pwd:" + String(_password));
    WiFi.begin(_ssid, _password);
    while ((WiFi.status() != WL_CONNECTED))
    {
        WiFi.begin(_ssid, _password);
        delay(3000);
        Serial.print("connecting...");
        Serial.println(i++);
    }
    Serial.println("wifi connected");
    openWiFi = true;
}

void remote()
{
    startWiFi();
    startMQTT();
}

void setup()
{
#ifdef ESP32S2
    USB.begin();
#endif
    Serial.begin(115200);
    while (!Serial)
        ;
    delay(1500);
    Serial.println("setup...");
    setting();
}

void loop()
{
    if (openWiFi)
        client.loop();
    appLoop();
}

#endif