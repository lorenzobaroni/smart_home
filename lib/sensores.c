#include "sensores.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"
#include "leds.h"
#include <stdio.h>

#define PIN_TCRT 15  // GPIO digital
#define LDR_ADC  28

bool sensor_luz_ativo = false;
bool sensor_presenca_ativo = false;

bool controle_manual_led_red = false;

void sensores_init() {
    gpio_init(PIN_TCRT);
    gpio_set_dir(PIN_TCRT, GPIO_IN);
    adc_init();
    adc_gpio_init(LDR_ADC);
}

int ler_luz() {
    adc_select_input(0);
    return adc_read();
}

bool detectar_presenca() {
    return gpio_get(PIN_TCRT) == 0;  // sensor refletivo ativo no LOW
}

void atualizar_estado_automatico() {
    if (!sensor_luz_ativo || !sensor_presenca_ativo) return;

    if (controle_manual_led_red) return;

    int luz = ler_luz();
    bool escuro = luz < 800;
    bool movimento = detectar_presenca();

    if (escuro && movimento) {
        set_led_red(true);
    } else {
        set_led_red(false);
    }
}
