#include <remote.h>
#include <led5x5.h>

uint8_t i = 0;
void onMsg(String msg)
{
    int8_t j = (++i % 25) - 12;
    j = j < 0 ? -1 * j : j;
    ws2812_setLED(12, 0, j, 0);
    ws2812_update();
}

void setting()
{
    ws2812_setLED(12, 0, 10, 0, true);
    remote();
    ws2812_setLED(12, 10, 0, 0, true);
    Serial.println("start...");
}
