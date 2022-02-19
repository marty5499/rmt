#include <remote.h>
#include <led5x5.h>

bool TimerHandler1(void *timer)
{
    sw = !sw;
    // updateLED();
    Serial.print("state:");
    Serial.println(sw);
    return true;
}

void hello_task(void *pvParameter)
{
    while (true)
    {
        updateLED();
        vTaskDelay(200 / portTICK_RATE_MS);
    }
}

void setting()
{
    ws2812_init();
    // i2s_start(I2S_NUM);
    flash(3, 0, 0, true);
    Serial.println("setting...");
    // ITimer0.attachInterruptInterval(TIMER0_INTERVAL_MS * 1000, TimerHandler1);
    xTaskCreate(&hello_task, "hello_task", 2048, NULL, 5, NULL);
    Serial.println("Start timer...");
    delay(1000);
    flash(0, 0, 5, false);
    // updateLED();
}
