#include <remote.h>
#include <led5x5.h>

uint8_t i = 0;
void onMsg(String msg)
{
    // Serial.println(msg);
    int8_t j = (++i % 25) - 12;
    j = j < 0 ? -1 * j : j;
    ws2812_init(false);
    flash(12, 0, j, 0, true);
    ws2812_deinit();
}

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