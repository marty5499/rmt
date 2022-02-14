#include <WiFi.h>
#include <esp32-hal-cpu.h>
#include "WS2812Led.h"

#include <PubSubClient.h>
#include <main.h>

WS2812Led wsLED(18 /*pin*/, 25 /*num*/);
USBCDC USBSerial;
bool refresh = false;

// WiFi
const char *_ssid = "webduino.io";
const char *_password = "webduino";
const char *mqtt_server = "mqtt1.webduino.io";
WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;
bool debugState = true;
bool sw = true;

ESP32Timer ITimer0(0);

void IRAM_ATTR ctrlWS2812()
{
  if (!refresh)
    return;
  refresh = false;
  if (sw)
    wsLED.UpdateAll(wsLED.RED);
  else
    wsLED.UpdateAll(wsLED.BLUE);
}

void debugMsg(const char *msg)
{
  if (debugState)
  {
    Serial.println(msg);
  }
}

void debugStrMsg(String str)
{
  if (debugState)
  {
    Serial.println(str);
  }
}

void callback(char *topic, byte *message, unsigned int length)
{
  String messageTemp;
  for (int i = 0; i < length; i++)
  {
    messageTemp += (char)message[i];
  }
  // debugStrMsg(messageTemp);
  sw = !sw;
  refresh = true;
}

void startMQTT()
{
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  if (client.connect("mqttTest", "webduino", "webduino"))
  {
    debugMsg("mqtt connected");
    client.subscribe("test5499");
  }
}

void startWiFi()
{
  WiFi.setAutoConnect(false);
  WiFi.disconnect(true, true);
  WiFi.setSleep(false);
  WiFi.setHostname("ESP32-Test");
  WiFi.begin(_ssid, _password);
  // Warte auf Verbindung
  while ((WiFi.status() != WL_CONNECTED))
  {
    WiFi.begin(_ssid, _password);
    delay(3000);
    debugStrMsg("connecting...");
  }
  debugStrMsg("wifi connected.");
}

void debugMode(bool t)
{
  debugState = t;
  if (debugState)
  {
    USB.begin();
    Serial.begin(115200);
  }
}

void remote()
{
  startWiFi();
  startMQTT();
}

void setup()
{
  setCpuFrequencyMhz(80);
  debugMode(true);
  delay(2000);
  debugMsg("GoGoGo...");
  wsLED.Brightness(5);
  wsLED.UpdateAll(wsLED.RED);
  //*
  remote();
  wsLED.UpdateAll(wsLED.GREEN);
  ITimer0.attachInterruptInterval(10 * TIMER0_INTERVAL_MS, ctrlWS2812);
  //*/
}

void loop()
{
  client.loop();
}
