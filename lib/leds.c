#include "leds.h"
#include "hardware/gpio.h"

#define LED_R 13
#define LED_G 11
#define LED_B 12

void gpio_led_init() {
    gpio_init(LED_R); gpio_set_dir(LED_R, GPIO_OUT); gpio_put(LED_R, 0);
    gpio_init(LED_G); gpio_set_dir(LED_G, GPIO_OUT); gpio_put(LED_G, 0);
    gpio_init(LED_B); gpio_set_dir(LED_B, GPIO_OUT); gpio_put(LED_B, 0);
}

void set_led_red(bool on)    { gpio_put(LED_R, on); }
void set_led_green(bool on)  { gpio_put(LED_G, on); }
void set_led_blue(bool on)   { gpio_put(LED_B, on); }
