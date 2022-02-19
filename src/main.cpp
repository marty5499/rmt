#include <remote.h>
#include <led5x5.h>

void onMsg(String msg)
{
    Serial.println(msg);
    sw = !sw;
    flash(0, sw ? 1 : 3, 0, false);
}

void IRAM_ATTR refreshLED(void *pvParameter)
{
    while (true)
    {
        updateLED();
        vTaskDelay(30 / portTICK_RATE_MS);
    }
}

void setting()
{
    ws2812_init();
    remote();
    flash(5, 0, 0, true);
    Serial.println("setting...");
    xTaskCreate(&refreshLED, "hello_task", 8192, NULL, 7, NULL);
    delay(2000);
    flash(0, 0, 5, false);
}

void appLoop()
{
    // updateLED();
}