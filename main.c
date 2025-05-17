#include <stdio.h>                  
#include "pico/stdlib.h"            
#include "hardware/adc.h"           
#include "pico/cyw43_arch.h"        // Biblioteca para controle da conexão Wi-Fi (CYW43)

// Bibliotecas do projeto
#include "lib/leds.h"               // Controle dos LEDs RGB
#include "lib/sensores.h"           // Controle dos sensores (TCRT5000 + LDR)
#include "lib/webserver.h"          // Servidor web embarcado

int main() {
    stdio_init_all();

    gpio_led_init();
    sensores_init();

    // Inicializa o driver do Wi-Fi
    if (cyw43_arch_init()) {
        printf("Erro ao inicializar o Wi-Fi\n");
        return -1;
    }

    // Coloca o Wi-Fi no modo estação (conecta-se a um roteador)
    cyw43_arch_enable_sta_mode();

    // Tenta conectar-se à rede Wi-Fi especificada (SSID e senha)
    if (cyw43_arch_wifi_connect_timeout_ms("Baroni VIVO", "baroni123456789", CYW43_AUTH_WPA2_AES_PSK, 30000)) {
        printf("Erro ao conectar no Wi-Fi\n");
        return -1;
    }

    printf("Conectado com sucesso!\n");

    // Inicia o servidor web (disponível na rede local via navegador)
    iniciar_webserver();

    // Loop principal do programa embarcado
    while (true) {
        cyw43_arch_poll();                // Mantém a pilha de rede ativa
        atualizar_estado_automatico();    
        sleep_ms(100);                    
    }

    // Encerra a arquitetura Wi-Fi
    cyw43_arch_deinit();
    return 0;
}
