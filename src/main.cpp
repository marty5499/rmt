#include <Arduino.h>
#include <WiFi.h>
#include <esp32-hal-cpu.h>
#include "WS2812Led.h"

#include <PubSubClient.h>
#include <main.h>

static const uint8_t WS2812LedPin = 18;
WS2812Led wsLED(WS2812LedPin, 25);

// WiFi
const char *_ssid = "webduino.io";
const char *_password = "webduino";
const char *mqtt_server = "mqtt1.webduino.io";
long cnt = 0;

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;
bool debugState = true;
bool sw = true;

USBCDC USBSerial;
ESP32Timer ITimer0(0);

void TimerHandler0()
{
  // USBSerial.print("ITimer0: millis() = ");
  // USBSerial.println(millis());
  //*
  if (sw)
    wsLED.UpdateAll(wsLED.RED);
  else
    wsLED.UpdateAll(wsLED.BLUE);
  //*/
}

void debugMsg(const char *msg)
{
  if (debugState)
  {
    USBSerial.println(msg);
  }
}

void debugStrMsg(String str)
{
  if (debugState)
  {
    USBSerial.println(str);
  }
}

void callback(char *topic, byte *message, unsigned int length)
{
  String messageTemp;
  for (int i = 0; i < length; i++)
  {
    messageTemp += (char)message[i];
  }
  debugStrMsg(messageTemp);
  sw = !sw;
}

void startMQTT()
{
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  if (client.connect("mqttTest", "webduino", "webduino"))
  {
    debugMsg("connected");
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
  if (WiFi.status() == WL_CONNECTED)
  {
    // debugMsg("" + WiFi.localIP());
  }
  else
  {
    debugStrMsg("\n ERROR: WiFi Connection not possible!\n");
  }
}

void debugMode(bool t)
{
  debugState = t;
  if (debugState)
  {
    USB.begin();
    USBSerial.begin(115200);
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
  debugMsg("GoGoGo...");
  wsLED.Brightness(3);
  wsLED.UpdateAll(wsLED.RED);
  delay(100);
  remote();
  wsLED.UpdateAll(wsLED.GREEN);
  ITimer0.attachInterruptInterval(TIMER0_INTERVAL_MS * 10, TimerHandler0);
}

void loop()
{
  client.loop();
}
