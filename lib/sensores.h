#ifndef SENSORES_H
#define SENSORES_H

#include <stdbool.h>

extern bool sensores_ativos;
extern bool controle_manual_led_red;

void sensores_init();
int ler_luz();
bool detectar_presenca();
void atualizar_estado_automatico();

#endif
