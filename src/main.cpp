#include <remote.h>
#include <led5x5.h>

void IRAM_ATTR onMsg(String msg)
{
    //Serial.println(msg);
    //sw = !sw;
    //flash(0, sw ? 1 : 5, 0, true);
    //refresh = true;
}

void IRAM_ATTR refreshLED(void *pvParameter)
{
    while (true)
    {
        refresh = true;
        if (refresh)
        {
            // updateLED();
            refresh = false;
            sw = !sw;
            flash(0, sw ? 1 : 5, 0, true);
        }
        vTaskDelay(60 / portTICK_RATE_MS);
    }
}

/**
  1. 不打開 wifi , 不重置 i2s , 會有固定閃燈

**/
void setting()
{
    ws2812_init();
    flash(0, 5, 0, true);
    //remote();
    Serial.println("setting...");
    //xTaskCreate(&refreshLED, "led_refresh", 8192, NULL, configMAX_PRIORITIES - 1, NULL);
    delay(1000);
    flash(0, 0, 5, true);
}

void appLoop()
{
    //*
    sw = !sw;
    if (sw)
        flash(0, 7, 0, true);
    else
        flash(0, 0, 7, true);
    //*/
}