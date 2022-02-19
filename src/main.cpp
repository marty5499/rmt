#include <remote.h>
#include <led5x5.h>

uint8_t i = 0;
void /*IRAM_ATTR*/ onMsg(String msg)
{
    i = ++i % 25;
    refresh = true;
}

void /*IRAM_ATTR*/ refreshLED(void *pvParameter)
{
    while (true)
    {
        //*
        for (int i = -10; i < 10; i++)
        {
            if (refresh)
            {
                ws2812_init();
                flash(12, 0, i < 0 ? (-1 * i) << 1 : i << 1, 0, true);
                ws2812_deinit();
                refresh = false;
            }
            delay(40);
        }
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
    ws2812_init();
    flash(12, 0, 10, 0, true);
    remote();
    Serial.println("setting...");
    flash(12, 10, 0, 0, true);
    ws2812_deinit();
    xTaskCreate(&refreshLED, "led_refresh", 2048, NULL, configMAX_PRIORITIES - 1, NULL);
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