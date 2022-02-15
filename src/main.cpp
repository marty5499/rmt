#include "main.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp32-hal.h"
#include "driver/rmt.h"

USBCDC USBSerial;
#define NR_OF_LEDS 10
#define NR_OF_ALL_BITS 24 * NR_OF_LEDS

//

rmt_data_t led_data[NR_OF_ALL_BITS];
rmt_obj_t *rmt_send = NULL;

int color[] = {0x0, 0x0, 0x5}; // GRB value

void flash(int led_index)
{
    // Init data with only one led ON
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
                    led_data[i].duration0 = 8;
                    led_data[i].level1 = 0;
                    led_data[i].duration1 = 4;
                }
                else
                {
                    led_data[i].level0 = 1;
                    led_data[i].duration0 = 4;
                    led_data[i].level1 = 0;
                    led_data[i].duration1 = 8;
                }
                i++;
            }
        }
    }
    // make the led travel in the pannel
    if ((++led_index) >= NR_OF_LEDS)
    {
        led_index = 0;
    }
}

void setup()
{
    USB.begin();
    Serial.begin(115200);
    delay(1000);
    Serial.println("GoGOGo...");
    if ((rmt_send = rmtInit(18, RMT_TX_MODE, RMT_MEM_256)) == NULL)
    {
        Serial.println("init sender failed\n");
    }
    float realTick = rmtSetTick(rmt_send, 100);
    Serial.printf("real tick set to: %fns\n", realTick);
    // Send the data
    // rmtWrite(rmt_send, led_data, NR_OF_ALL_BITS);
    Serial.println("fill led");
    rmt_set_tx_intr_en(RMT_CHANNEL_0, false);
    delay(10);
    for (int i = 0; i < NR_OF_LEDS; i++)
    {
        Serial.print("flash:");
        Serial.println(i);
        flash(i);
        //*/
        //rmtLoop(rmt_send, led_data, NR_OF_ALL_BITS);
        //rmt_write_items(RMT_CHANNEL_0, (const rmt_item32_t *)led_data, NR_OF_ALL_BITS, false);
        //*/
        //rmt_tx_memory_reset(RMT_CHANNEL_0);
        rmt_tx_start(RMT_CHANNEL_0, false);
        delay(10);
        Serial.print("NR_OF_ALL_BITS:");
        Serial.println(NR_OF_ALL_BITS);
        rmt_fill_tx_items(RMT_CHANNEL_0, (const rmt_item32_t *)led_data, NR_OF_ALL_BITS, 0);
        delay(10);
        //*/
        delay(500);
    }
        rmt_tx_stop(RMT_CHANNEL_0);
    Serial.println("done.");
}

void loop()
{
}