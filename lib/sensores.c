#include "sensores.h"          
#include "hardware/gpio.h"     
#include "hardware/adc.h"      
#include "leds.h"              
#include <stdio.h>

// Define os pinos conectados aos sensores
#define PIN_TCRT 16      
#define LDR_ADC  28      
// Variável global para controlar se os sensores estão ativos
bool sensores_ativos = false;

// Flag usada para desativar o modo automático caso o usuário tenha acionado algum LED manualmente
bool controle_manual_led_red = false;

// Inicializa os sensores: configura o pino digital e o ADC
void sensores_init() {

    gpio_init(PIN_TCRT);
    gpio_set_dir(PIN_TCRT, GPIO_IN);
    gpio_pull_up(PIN_TCRT);  // Ativa resistor de pull-up interno

    adc_init();
    adc_gpio_init(LDR_ADC);
}

// Realiza a leitura do sensor de luz (LDR)
int ler_luz() {
    adc_select_input(2);     
    return adc_read();      
}

// Verifica se há presença detectada pelo sensor TCRT5000
bool detectar_presenca() {
    // O sensor retorna LOW (0) quando detecta um objeto (reflexão)
    return gpio_get(PIN_TCRT) == 0;
}

// Função chamada continuamente para atualizar o estado automático da iluminação
void atualizar_estado_automatico() {
    // Só executa se os sensores estiverem ativos e o modo manual não estiver interferindo
    if (!sensores_ativos) return;
    if (controle_manual_led_red) return;

    // Faz leitura da luminosidade e verifica presença
    int luz = ler_luz();
    bool escuro = luz < 800;           // Define se o ambiente está escuro
    bool movimento = detectar_presenca(); // Verifica se há presença

    // Se o ambiente estiver escuro e alguém estiver presente, acende todos os LEDs
    if (escuro && movimento) {
        set_led_red(true);
        set_led_green(true);
        set_led_blue(true);
    } else {
        // Caso contrário, apaga todos
        set_led_red(false);
        set_led_green(false);
        set_led_blue(false);
    }
}
