#include <Arduino.h>
#include <WiFi.h>
#include <esp32-hal-cpu.h>
/*
//#define FASTLED_ESP32_I2S true
//#define FASTLED_RMT_SERIAL_DEBUG 1
#define FASTLED_RMT_MEM_BLOCKS 2
#define FASTLED_RMT_MAX_CHANNELS 1
#define FASTLED_RMT_MAX_CONTROLLERS 1
#define FASTLED_ESP32_FLASH_LOCK 1
#define FASTLED_RMT_MEM_WORDS_PER_CHANNEL 64
#define FASTLED_ALLOW_INTERRUPTS 0
#define FASTLED_INTERRUPT_RETRY_COUNT 0
//*/

#include <FastLED.h>
#include <PubSubClient.h>
#include <main.h>

#define NUM_LEDS 200
#define DATA_PIN 18
const char *_ssid = "KingKit_2.4G";
const char *_password = "webduino";
CRGB leds[NUM_LEDS];

const char *mqtt_server = "mqtt1.webduino.io";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

void callback(char *topic, byte *message, unsigned int length)
{
  /*
  USBSerial.print("Msgs: ");
  USBSerial.print(topic);
  USBSerial.print(". Message: ");
  String messageTemp;
  for (int i = 0; i < length; i++)
  {
    USBSerial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  USBSerial.println();
  //*/
  for (byte i = 0; i < 25; i++)
  {
    leds[i] = 0;
    FastLED.show();
  }
  if (++value % 2 == 0)
  {
    // USBSerial.println("on");
    leds[12] = 0x030000;
  }
  else
  {
    // USBSerial.println("off");
    leds[12] = 0x000000;
  }
  FastLED.show();
}

void startMQTT()
{
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  if (client.connect("mqttTest", "webduino", "webduino"))
  {
    USBSerial.println("connected");
    client.subscribe("test5499");
  }
}

void startWiFi()
{
  WiFi.setAutoConnect(false);
  WiFi.disconnect(true, true);
  WiFi.setSleep(false);
  USBSerial.println("\n");
  USBSerial.print("\n\n WiFi.AutoConnect: ");
  USBSerial.println(WiFi.getAutoConnect());
  USBSerial.println("### === ESP32 Wifi-Test min v1.0 === ###");
  USBSerial.printf(" WiFi mode set to WIFI_STA: %s\n", WiFi.mode(WIFI_STA) ? "Success!" : "Failed!");
  WiFi.setHostname("ESP32-Test");
  WiFi.begin(_ssid, _password);
  // Warte auf Verbindung
  while ((WiFi.status() != WL_CONNECTED))
  {
    WiFi.begin(_ssid, _password);
    delay(3000);
    USBSerial.print(" ");
    USBSerial.print(WiFi.status());
  }
  if (WiFi.status() == WL_CONNECTED)
  {
    USBSerial.println("");
    USBSerial.print(" Verbunden mit: ");
    USBSerial.println(WiFi.SSID());
    USBSerial.print(" IP address: ");
    USBSerial.println(WiFi.localIP());
    USBSerial.print("\n");
    USBSerial.print(" WiFi.getHostname: ");
    USBSerial.println(WiFi.getHostname());
    USBSerial.print("\n");
  }
  else
  {
    USBSerial.println("\n ERROR: WiFi Connection not possible!\n");
  }
}

void clearLED()
{
  for (byte i = 0; i < 25; i++)
  {
    leds[i] = 0;
  }
  FastLED.show();
}

void setup()
{
  setCpuFrequencyMhz(240);
  USB.begin();
  USBSerial.begin(115200);
  USBSerial.println("GoGoGo...");
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  FastLED.clearData();
  FastLED.clear();
  startWiFi();
  startMQTT();
}

void loop()
{
  client.loop();
}
