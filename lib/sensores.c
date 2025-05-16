#include "sensores.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"
#include "leds.h"
#include <stdio.h>

#define PIN_TCRT 16      // Novo pino digital para sensor TCRT5000
#define LDR_ADC  28      // Novo pino analógico para o LDR (ADC2)

bool sensor_luz_ativo = false;
bool sensor_presenca_ativo = false;
bool controle_manual_led_red = false;

void sensores_init() {
    // Configuração do TCRT5000
    gpio_init(PIN_TCRT);
    gpio_set_dir(PIN_TCRT, GPIO_IN);
    gpio_pull_up(PIN_TCRT);  // Pull-up interno ativado

    // Configuração do ADC para o LDR
    adc_init();
    adc_gpio_init(LDR_ADC);  // Habilita o GPIO28 como entrada ADC
}

int ler_luz() {
    adc_select_input(2);     // ADC2 = GPIO28
    return adc_read();       // Retorna valor entre 0 e 4095
}

bool detectar_presenca() {
    return gpio_get(PIN_TCRT) == 0;  // Reflexão detectada (ativo em LOW)
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
