#include <Arduino.h>
#include <WiFi.h>
#include <esp32-hal-cpu.h>
//*
//#define FASTLED_ESP32_I2S true
//#define FASTLED_RMT_SERIAL_DEBUG 1
//#define FASTLED_ESP32_FLASH_LOCK 1
//#define FASTLED_ALLOW_INTERRUPTS 0
//#define FASTLED_INTERRUPT_RETRY_COUNT 0
#define FASTLED_RMT_MEM_BLOCKS 2
#define FASTLED_RMT_MAX_CHANNELS 1
#define FASTLED_RMT_MAX_CONTROLLERS 1
#define FASTLED_RMT_MEM_WORDS_PER_CHANNEL 64
#define CONFIG_IDF_TARGET_ESP32S2 1
//*/

#include <FastLED.h>
#include <PubSubClient.h>
#include <main.h>

#define NUM_LEDS 25
#define DATA_PIN 18
const char *_ssid = "webduino.io";
const char *_password = "webduino";
CRGB leds[NUM_LEDS];

const char *mqtt_server = "mqtt1.webduino.io";
long cnt = 0;

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;
bool debugState = true;

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
  if (++value % 2 == 0)
    leds[12] = 0x080000;
  else
    leds[12] = 0x000500;
  FastLED.show();
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

void flashLed()
{
  FastLED.clear();
  FastLED.show();
  while (true)
  {
    leds[12] = 0x050000;
    FastLED.show();
    delay(100);
    leds[12] = 0x000500;
    FastLED.show();
    delay(100);
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

void local()
{
  flashLed();
}

void remote()
{
  startWiFi();
  startMQTT();
}

void initLED(uint32_t color)
{
  for (byte i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = color;
  }
  FastLED.show();
}

void setup()
{
  setCpuFrequencyMhz(240);
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  FastLED.clear();
  debugMode(true);
  debugMsg("GoGoGo...");
  pinMode(DATA_PIN, OUTPUT);
  initLED(0x050000);
  remote();
  //local();
  initLED(0x000200);
  //USBSerial.print(">>>>>>");
  //USBSerial.println(MAX_PULSES);
}

void loop()
{
  client.loop();
}
