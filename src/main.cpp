#include <WiFi.h>
#include <PubSubClient.h>
#include <main.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp32-hal.h"
#include "ESP32TimerInterrupt.h"
///// rmt //////
ESP32Timer ITimer0(0);

///// rmt //////
#define NR_OF_LEDS 25
#define NR_OF_ALL_BITS 24 * NR_OF_LEDS
rmt_data_t led_data[NR_OF_ALL_BITS];
rmt_obj_t *rmt_send = NULL;
int color[] = {0x5, 0x1, 0x7}; // RGB value
int led_index = 0;

///// usb //////
USBCDC USBSerial;
bool refresh = false;

// WiFi
const char *_ssid = "webduino.io";
const char *_password = "webduino";
const char *mqtt_server = "mqtt1.webduino.io";
WiFiClient espClient;
PubSubClient client(espClient);
char msg[50];
int value = 0;
bool sw = true;
long lastMsg = 0;
bool debugState = true;

void rmtSetup()
{
    rmt_send = rmtInit(18, RMT_TX_MODE, RMT_MEM_256);
    float realTick = rmtSetTick(rmt_send, 100); // 100ns
    Serial.printf("real tick set to: %fns\n", realTick);
}

void flash()
{
    int led, col, bit;
    int i = 0;
    for (led = 0; led < NR_OF_LEDS; led++)
    {
        for (col = 0; col < 3; col++)
        {
            for (bit = 0; bit < 8; bit++)
            {
                if ((color[col] & (1 << (7 - bit))) && (led == led_index))
                {
                    led_data[i].level0 = 1;
                    led_data[i].duration0 = 9;
                    led_data[i].level1 = 0;
                    led_data[i].duration1 = 9;
                }
                else
                {
                    led_data[i].level0 = 1;
                    led_data[i].duration0 = 3;
                    led_data[i].level1 = 0;
                    led_data[i].duration1 = 9;
                }
                i++;
            }
        }
    }
    if ((++led_index) >= NR_OF_LEDS)
    {
        led_index = 0;
    }
    rmtWrite(rmt_send, led_data, NR_OF_ALL_BITS);
}

void TimerHandler0()
{
    if (refresh)
    {
        flash();
        refresh = false;
    }
}

void callback(char *topic, byte *message, unsigned int length)
{
    String messageTemp;
    for (int i = 0; i < length; i++)
    {
        messageTemp += (char)message[i];
    }
    // Serial.println(messageTemp);
    sw = !sw;
    refresh = true;
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
    WiFi.setAutoConnect(false);
    WiFi.disconnect(true, true);
    WiFi.setSleep(false);
    WiFi.setHostname("ESP32-Test");
    WiFi.begin(_ssid, _password);
    while ((WiFi.status() != WL_CONNECTED))
    {
        WiFi.begin(_ssid, _password);
        delay(3000);
        Serial.println("connecting...");
    }
    Serial.println("wifi connected.");
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
    setCpuFrequencyMhz(240);
    debugMode(true);
    delay(2000);
    rmtSetup();
    Serial.println("GoGoGo...");
    remote();
    ITimer0.attachInterruptInterval(10 * 1000, TimerHandler0);
}

void loop()
{
    client.loop();
}
