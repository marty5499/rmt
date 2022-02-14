#include "main.h"
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "driver/gpio.h"
#include "driver/rmt.h"
#include "esp_log.h"

USBCDC USBSerial;

void rgb(uint8_t r, uint8_t g, uint8_t b)
{
  uint8_t idx = 0;
  for (int8_t i = 7; i > -1; i--)
  {
    uint8_t x = (g >> i) & 1;
    if (x)
    {
      rmt_item32_t bit[] = {{{{14, 1, 12, 0}}}};
      ESP_ERROR_CHECK(rmt_fill_tx_items(RMT_CHANNEL_0, bit, 1, idx++));
    }
    else
    {
      rmt_item32_t bit[] = {{{{7, 1, 16, 0}}}};
      ESP_ERROR_CHECK(rmt_fill_tx_items(RMT_CHANNEL_0, bit, 1, idx++));
    }
  }

  for (int8_t i = 7; i > -1; i--)
  {
    uint8_t x = (r >> i) & 1;
    if (x)
    {
      rmt_item32_t bit[] = {{{{14, 1, 12, 0}}}};
      ESP_ERROR_CHECK(rmt_fill_tx_items(RMT_CHANNEL_0, bit, 1, idx++));
    }
    else
    {
      rmt_item32_t bit[] = {{{{7, 1, 16, 0}}}};
      ESP_ERROR_CHECK(rmt_fill_tx_items(RMT_CHANNEL_0, bit, 1, idx++));
    }
  }

  for (int8_t i = 7; i > -1; i--)
  {
    uint8_t x = (b >> i) & 1;
    if (x)
    {
      rmt_item32_t bit[] = {{{{14, 1, 12, 0}}}};
      ESP_ERROR_CHECK(rmt_fill_tx_items(RMT_CHANNEL_0, bit, 1, idx++));
    }
    else
    {
      rmt_item32_t bit[] = {{{{7, 1, 16, 0}}}};
      ESP_ERROR_CHECK(rmt_fill_tx_items(RMT_CHANNEL_0, bit, 1, idx++));
    }
  }

  rmt_tx_start(RMT_CHANNEL_0, 1);
  rmt_wait_tx_done(RMT_CHANNEL_0, pdMS_TO_TICKS(5)); //tick = 100/80M=1.25us
  ets_delay_us(10);
  rmt_tx_stop(RMT_CHANNEL_0);
}

void setup(void)
{
  rmt_config_t cfg = RMT_DEFAULT_CONFIG_TX(GPIO_NUM_18, RMT_CHANNEL_0);
  cfg.clk_div = 2;

  USB.begin();
  Serial.begin(115200);
  delay(500);
  Serial.println("GoGoGo...");
  rmt_config(&cfg);
  rmt_driver_install(cfg.channel, 0, 0);
  for (int i = 0; i < 10; i++)
  {
    rgb(5, 0, 0);
    delay(500);
    Serial.println("next...");
    rgb(0, 5, 0);
    delay(500);
    Serial.println("next...");
    rgb(0, 0, 5);
    delay(500);
  }
}

void loop()
{
}