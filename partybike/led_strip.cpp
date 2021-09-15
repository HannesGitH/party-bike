
#include "led_strip.h"
//#include "freertos/task.h"
#include <Arduino.h>

#include <string.h>

#define LED_STRIP_TASK_SIZE             (4096)
#define LED_STRIP_TASK_PRIORITY         (configMAX_PRIORITIES - 1)

#define LED_STRIP_REFRESH_PERIOD_MS     (60U) // TODO: add as parameter to led_strip_init

#define LED_STRIP_NUM_RMT_ITEMS_PER_LED (24U) // Assumes 24 bit color for each led

// RMT Clock source is @ 80 MHz. Dividing it by 8 gives us 10 MHz frequency, or 100ns period.
#define LED_STRIP_RMT_CLK_DIV (4) // TODO XXX hier iwas? 4 machte schonmal mehr als 8 was eigtl unlogisch ist, wird das RMT peripheral vllt schon anderwaltig (zb durch BTSerial) verwendet?

/****************************
        WS2812b Timing
 ****************************/
#define LED_STRIP_RMT_TICKS_BIT_1_HIGH_WS2812 16    // 800ns (900ns +/- 150ns per datasheet)
#define LED_STRIP_RMT_TICKS_BIT_1_LOW_WS2812  9     // 450ns (350ns +/- 150ns per datasheet)
#define LED_STRIP_RMT_TICKS_BIT_0_HIGH_WS2812 8     // 400ns (350ns +/- 150ns per datasheet)
#define LED_STRIP_RMT_TICKS_BIT_0_LOW_WS2812  17    // 850ns (900ns +/- 150ns per datasheet)

/****************************
        WS2812 Timing
 ****************************/
// #define LED_STRIP_RMT_TICKS_BIT_1_HIGH_WS2812 14    // 700ns (900ns +/- 150ns per datasheet)
// #define LED_STRIP_RMT_TICKS_BIT_1_LOW_WS2812  12    // 600ns (350ns +/- 150ns per datasheet)
// #define LED_STRIP_RMT_TICKS_BIT_0_HIGH_WS2812 7     // 350ns (350ns +/- 150ns per datasheet)
// #define LED_STRIP_RMT_TICKS_BIT_0_LOW_WS2812  16    // 800ns (900ns +/- 150ns per datasheet)

// Function pointer for generating waveforms based on different LED drivers
typedef void (*led_fill_rmt_items_fn)(irgb_t *led_strip_buf, rmt_item32_t *rmt_items, uint32_t led_strip_length);

static inline void led_strip_fill_item_level(rmt_item32_t* item, int high_ticks, int low_ticks)
{
    item->level0 = 1;
    item->duration0 = high_ticks;
    item->level1 = 0;
    item->duration1 = low_ticks;
}

static inline void led_strip_rmt_bit_1_ws2812(rmt_item32_t* item)
{
    led_strip_fill_item_level(item, LED_STRIP_RMT_TICKS_BIT_1_HIGH_WS2812, LED_STRIP_RMT_TICKS_BIT_1_LOW_WS2812);
}

static inline void led_strip_rmt_bit_0_ws2812(rmt_item32_t* item)
{
    led_strip_fill_item_level(item, LED_STRIP_RMT_TICKS_BIT_0_HIGH_WS2812, LED_STRIP_RMT_TICKS_BIT_0_LOW_WS2812);
}

static void led_strip_fill_rmt_items_ws2812(irgb_t *led_strip_buf, rmt_item32_t *rmt_items, uint32_t led_strip_length)
{
    uint32_t rmt_items_index = 0;
    for (uint32_t led_index = 0; led_index < led_strip_length; led_index++) {
        irgb_t IRGB = led_strip_buf[led_index];
        uint32_t led_colors = IRGB.r<<16 | IRGB.g<<8 | IRGB.b;
        for (uint8_t bit = 24; bit != 0; bit--) {
            uint8_t bit_set = (led_colors >> (bit - 1)) & 1;
            if(bit_set) {
                led_strip_rmt_bit_1_ws2812(&(rmt_items[rmt_items_index]));
            } else {
                led_strip_rmt_bit_0_ws2812(&(rmt_items[rmt_items_index]));
            }
            rmt_items_index++;
        }
    }
}

static void led_strip_task(void *arg)
{
    struct led_strip_t *led_strip = (struct led_strip_t *)arg;
    led_fill_rmt_items_fn led_make_waveform = NULL;
    bool make_new_rmt_items = true;

    size_t num_items_malloc = (LED_STRIP_NUM_RMT_ITEMS_PER_LED * led_strip->led_strip_length);
    rmt_item32_t *rmt_items = (rmt_item32_t*) malloc(sizeof(rmt_item32_t) * num_items_malloc);
    if (!rmt_items) {
        vTaskDelete(NULL);
    }

    led_make_waveform = led_strip_fill_rmt_items_ws2812;

    for(;;) {
        configASSERT(led_strip->access_semaphore);
        rmt_wait_tx_done(led_strip->rmt_channel, portMAX_DELAY);
        xSemaphoreTake(led_strip->access_semaphore, portMAX_DELAY); //this threw (xQueueGenericReceive)- assert failed!

        make_new_rmt_items = true;

        if (make_new_rmt_items) {
            led_make_waveform(led_strip->led_strip_buf, rmt_items, led_strip->led_strip_length);
        }

        rmt_write_items(led_strip->rmt_channel, rmt_items, num_items_malloc, false);
        xSemaphoreGive(led_strip->access_semaphore);
        vTaskDelay(LED_STRIP_REFRESH_PERIOD_MS / portTICK_PERIOD_MS);
    }

    if (rmt_items) {
        free(rmt_items);
    }
    vTaskDelete(NULL);
}

static bool led_strip_init_rmt(struct led_strip_t *led_strip)
{
    const rmt_tx_config_t tx_cfg = {
        .loop_en = false,
        .carrier_freq_hz = 100, // Not used, but has to be set to avoid divide by 0 err
        .carrier_duty_percent = 50,
        .carrier_level = RMT_CARRIER_LEVEL_LOW,
        .carrier_en = false,
        .idle_level = RMT_IDLE_LEVEL_LOW,
        .idle_output_en = true
    };
    rmt_config_t rmt_cfg = {
        .rmt_mode = RMT_MODE_TX,
        .channel = led_strip->rmt_channel,
        .clk_div = LED_STRIP_RMT_CLK_DIV,
        .gpio_num = led_strip->gpio,
        .mem_block_num = 1,
    };
    rmt_cfg.tx_config = tx_cfg;

    esp_err_t cfg_ok = rmt_config(&rmt_cfg);
    if (cfg_ok != ESP_OK) {
        return false;
    }
    esp_err_t install_ok = rmt_driver_install(rmt_cfg.channel, 0, 0);
    if (install_ok != ESP_OK) {
        return false;
    }

    return true;
}

bool led_strip_init(struct led_strip_t *led_strip)
{
    TaskHandle_t led_strip_task_handle;

    if ((led_strip == NULL) ||
        (led_strip->rmt_channel == RMT_CHANNEL_MAX) ||
        (led_strip->gpio > GPIO_NUM_33) ||  // only inputs above 33
        (led_strip->led_strip_buf == NULL) ||
        (led_strip->led_strip_length == 0) ||
        (led_strip->access_semaphore == NULL)) {
        return false;
    }

    memset(led_strip->led_strip_buf, 0, sizeof(irgb_t) * led_strip->led_strip_length);

    bool init_rmt = led_strip_init_rmt(led_strip);
    if (!init_rmt) {
        return false;
    }
    xSemaphoreGive(led_strip->access_semaphore);
    BaseType_t task_created = xTaskCreate(led_strip_task,
                                            "led_strip_task",
                                            LED_STRIP_TASK_SIZE,
                                            led_strip,
                                            LED_STRIP_TASK_PRIORITY,
                                            &led_strip_task_handle
                                         );

    if (!task_created) {
        return false;
    }

    return true;
}


bool led_strip_set_pixel_color(struct led_strip_t *led_strip, uint32_t pixel_num, irgb_t color)
{
    bool set_led_success = true;

    if ((!led_strip) || (pixel_num > led_strip->led_strip_length)) {
        return false;
    }

    xSemaphoreTake(led_strip->access_semaphore, portMAX_DELAY);
    led_strip->led_strip_buf[pixel_num] = color;
    //Serial.printf("turning pixel %d into {r: %d g:%d b: %d}\n", pixel_num, color.r, color.g, color.b);
    xSemaphoreGive(led_strip->access_semaphore);

    return set_led_success;
}

bool led_strip_set_pixel_rgb(struct led_strip_t *led_strip, uint32_t pixel_num, uint8_t red, uint8_t green, uint8_t blue)
{
    return led_strip_set_pixel_color(led_strip, pixel_num, iRGB(red,green,blue));
}

bool led_strip_get_pixel_color(struct led_strip_t *led_strip, uint32_t pixel_num, irgb_t * color)
{
    bool get_success = true;

    if ((!led_strip) ||
        (pixel_num > led_strip->led_strip_length)
    ){ 
        return false;
    }
    *color = led_strip->led_strip_buf[pixel_num];

    return get_success;
}

bool led_strip_addto_pixel_color(struct led_strip_t *led_strip, uint32_t pixel_num, irgb_t color)
{
    irgb_t current_color;
    if (led_strip_get_pixel_color(led_strip,pixel_num,&current_color))
    {   
        ////Serial.printf("coloring %d\t%d with \t {r: %d g:%d b: %d}\t+\t{r: %d g:%d b: %d}\t=\t{r: %d g:%d b: %d}\n", led_strip ,pixel_num, current_color.r, current_color.g, current_color.b, color.r,  color.g,  color.b,(color+current_color).r, (color+current_color).g, (color+current_color).b);
        return led_strip_set_pixel_color(led_strip,pixel_num,color+current_color);
    }
    return false;
}

/**
 * Clears the LED strip
 */
bool led_strip_clear(struct led_strip_t *led_strip)
{
    bool success = true;

    if (!led_strip) {
        return false;
    }

    xSemaphoreTake(led_strip->access_semaphore, portMAX_DELAY);
    memset(led_strip->led_strip_buf, 0, sizeof(irgb_t) * led_strip->led_strip_length);
    xSemaphoreGive(led_strip->access_semaphore);
    
    return success;
}
