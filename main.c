#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "pico/cyw43_arch.h"

#include "lib/leds.h"
#include "lib/sensores.h"
#include "lib/webserver.h"

int main() {
    stdio_init_all();
    gpio_led_init();
    sensores_init();

    if (cyw43_arch_init()) {
        printf("Erro ao inicializar o Wi-Fi\n");
        return -1;
    }

    cyw43_arch_enable_sta_mode();
    if (cyw43_arch_wifi_connect_timeout_ms("Baroni VIVO", "baroni123456789", CYW43_AUTH_WPA2_AES_PSK, 30000)) {
        printf("Erro ao conectar no Wi-Fi\n");
        return -1;
    }

    printf("Conectado com sucesso!\n");
    iniciar_webserver();

    while (true) {
        cyw43_arch_poll();
        atualizar_estado_automatico();
        sleep_ms(100);
    }

    cyw43_arch_deinit();
    return 0;
}
