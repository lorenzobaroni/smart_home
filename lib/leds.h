#ifndef LEDS_H
#define LEDS_H

#include <stdbool.h>

void gpio_led_init();
void set_led_red(bool on);
void set_led_green(bool on);
void set_led_blue(bool on);

#endif
