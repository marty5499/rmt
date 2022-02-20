#include "esp_system.h"
#include "driver/i2s.h"

#define LED_NUMBER 25
#define PIXEL_SIZE 12 // each colour takes 4 bytes
#define ZERO_BUFFER 50
#define SAMPLE_RATE (93750)
#define I2S_NUM (I2S_NUM_0)
#define I2S_DO_IO (18)
#define I2S_DI_IO (-1)

typedef struct
{
    uint8_t green;
    uint8_t red;
    uint8_t blue;
} ws2812_pixel_t;

i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
    .sample_rate = SAMPLE_RATE,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
    .channel_format = I2S_CHANNEL_FMT_ALL_RIGHT,
    .communication_format = (i2s_comm_format_t)(I2S_COMM_FORMAT_STAND_I2S | I2S_COMM_FORMAT_STAND_MSB),
    .dma_buf_count = 2,
    .dma_buf_len = LED_NUMBER * PIXEL_SIZE,
    .use_apll = true,
};

i2s_pin_config_t pin_config = {.bck_io_num = -1,
                               .ws_io_num = -1,
                               .data_out_num = I2S_DO_IO,
                               .data_in_num = -1};

ws2812_pixel_t led[LED_NUMBER];
static uint16_t size_buffer = LED_NUMBER * PIXEL_SIZE;
static uint8_t out_buffer[LED_NUMBER * PIXEL_SIZE] = {0x88};
static uint8_t off_buffer[ZERO_BUFFER] = {0x00};
static const uint16_t bitpatterns[4] = {0x88, 0x8e, 0xe8, 0xee};

void ws2812_init(bool setPin)
{
    i2s_driver_install(I2S_NUM, &i2s_config, 0, NULL);
    if (setPin)
        i2s_set_pin(I2S_NUM, &pin_config);
}

void ws2812_deinit()
{
    i2s_driver_uninstall(I2S_NUM);
}

void ws2812_update(ws2812_pixel_t *pixels)
{
    for (uint16_t i = 0; i < LED_NUMBER; i++)
    {
        int loc = i * PIXEL_SIZE;

        out_buffer[loc + 0] = bitpatterns[pixels[i].green & 0x03];
        out_buffer[loc + 1] = bitpatterns[(pixels[i].green >> 2) & 0x03];
        out_buffer[loc + 2] = bitpatterns[(pixels[i].green >> 4) & 0x03];
        out_buffer[loc + 3] = bitpatterns[(pixels[i].green >> 6) & 0x03];

        out_buffer[loc + 4] = bitpatterns[(pixels[i].red >> 0) & 0x03];
        out_buffer[loc + 5] = bitpatterns[(pixels[i].red >> 2) & 0x03];
        out_buffer[loc + 6] = bitpatterns[(pixels[i].red >> 4) & 0x03];
        out_buffer[loc + 7] = bitpatterns[(pixels[i].red >> 6) & 0x03];

        out_buffer[loc + 8] = bitpatterns[(pixels[i].blue >> 0) & 0x03];
        out_buffer[loc + 9] = bitpatterns[(pixels[i].blue >> 2) & 0x03];
        out_buffer[loc + 10] = bitpatterns[(pixels[i].blue >> 4) & 0x03];
        out_buffer[loc + 11] = bitpatterns[(pixels[i].blue >> 6) & 0x03];
    }
}

void updateLED()
{
    size_t bytes_written = 0;
    //i2s_start(I2S_NUM);
    i2s_write(I2S_NUM, out_buffer, size_buffer, &bytes_written, portMAX_DELAY);
    vTaskDelay(pdMS_TO_TICKS(10));
    i2s_zero_dma_buffer(I2S_NUM);
    //i2s_stop(I2S_NUM);
}

void flash(uint8_t n, uint8_t r, uint8_t g, uint8_t b, bool update)
{
    for (int i = 0; i < LED_NUMBER; i++)
    {
        if (i == n)
        {
            led[i].green = r;
            led[i].red = g;
            led[i].blue = b;
        }
        else
        {
            led[i].green = 0;
            led[i].red = 0;
            led[i].blue = 0;
        }
    }
    ws2812_update(led);
    if (update)
    {
        updateLED();
    }
}