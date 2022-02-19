#include <remote.h>
#include <led5x5.h>

uint8_t i = 0;
void IRAM_ATTR onMsg(String msg)
{
    i = ++i % 25;
    // flash(i, 0, 7, 0, true);
    // delay(10);
}

void IRAM_ATTR refreshLED(void *pvParameter)
{
    while (true)
    {
        //*
        for (int i = 0; i < 25; i++)
        {
            flash(i, 0, 7, 0, true);
            delay(10);
        }
        //*/
    }
}

/**
  1. 不打開 wifi , 不重置 i2s , 會有固定閃燈
     --> write 兩次 i2s 緩衝區可以解決 1
  2.

**/
void setting()
{
    ws2812_init();
    flash(12, 0, 10, 0, true);
    //remote();
    Serial.println("setting...");
    xTaskCreate(&refreshLED, "led_refresh", 8192, NULL, configMAX_PRIORITIES - 1, NULL);
    delay(1000);
    flash(12, 10, 0, 0, true);
    // ws2812_deinit();
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