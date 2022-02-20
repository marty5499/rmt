#include <main.h>
#include <led5x5.h>

uint8_t i = 0;
void onMsg(String msg)
{
    i = ++i % 25;
    int8_t j = i - 12;
    j = j < 0 ? -1 * j : j;
    ws2812_setLED(0, 0, j, 0);
    ws2812_setLED(4, 0, j, 0);
    ws2812_setLED(12, 0, j, 0);
    ws2812_setLED(20, 0, j, 0);
    ws2812_setLED(24, 0, j, 0);
    // ws2812_setAllLED(0, j, 0);
    ws2812_update();
}

void setting()
{
    ws2812_setAllLED(0, 10, 0, true);
    remote();
    ws2812_setAllLED(10, 0, 0, true);
    delay(1000);
    ws2812_setAllLED(1, 0, 1, true);
    Serial.println("start...");
}
