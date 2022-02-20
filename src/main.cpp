#include <remote.h>
#include <led5x5.h>

uint8_t i = 0;
void /*IRAM_ATTR*/ onMsg(String msg)
{
    //Serial.println(msg);
    refresh = true;
}

void IRAM_ATTR mqttCB(void *pvParameter)
{
    while (true)
    {
        while (!client.loop())
            ;
        refresh = true;
        delay(10);
    }
}

void IRAM_ATTR refreshLED(void *pvParameter)
{
    while (true)
    {
        client.loop();
        if (!refresh)
            continue;
        //delay(10);
        //Serial.println("proc msg..." + String(++i));
        int8_t j = (++i % 25) - 12;
        j = j < 0 ? -1 * j : j;
        //Serial.println("refresh..." + String(j));
        ws2812_init(false);
        flash(12, 0, j, 0, true);
        ws2812_deinit();
        //delay(10);
        refresh = false;
        //*/
    }
}

/**
  1. 不打開 wifi , 不重置 i2s , 會有固定閃燈
     --> write 兩次 i2s 緩衝區可以解決 1
  2. 打開 wifi , init / start / stop / deinit 可以減少閃燈

**/
void setting()
{
    pinMode(18, OUTPUT);
    digitalWrite(18, HIGH);
    pinMode(17, OUTPUT);
    digitalWrite(17, HIGH);
    ws2812_init(true);
    flash(12, 0, 10, 0, true);
    remote();
    Serial.println("setting...");
    flash(12, 10, 0, 0, true);
    ws2812_deinit();
    xTaskCreate(&refreshLED, "led_refresh", 16384, NULL, 6/*configMAX_PRIORITIES - 1*/, NULL);
    // xTaskCreate(&mqttCB, "mqtt_callback", 8192, NULL, 3, NULL);
}

void appLoop()
{
    /*
    for (int i = 0; i < 25; i++)
    {
        flash(i, 0, 7, 0, true);
        delay(50);
    }
    //*/
}