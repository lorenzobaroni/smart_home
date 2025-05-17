#include <stdio.h>             
#include <stdlib.h>            
#include <string.h>            
#include "lwip/tcp.h"         
#include "leds.h"              
#include "sensores.h"          
#include <stdbool.h>           

// Estado individual de cada LED
bool red_on = false;
bool green_on = false;
bool blue_on = false;

// Estado por "cômodo" controlado via interface web
bool quarto_on = false;
bool sala_on = false;
bool cozinha_on = false;

// Declarações das funções de callback para o servidor web
static err_t callback_aceitar(void *arg, struct tcp_pcb *newpcb, err_t err);
static err_t callback_receber(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err);

// Função que processa as requisições recebidas via navegador
void processar_requisicao(char *req) {
    // Alterna os LEDs do "quarto" (vermelho + verde)
    if (strstr(req, "GET /quarto")) {
        quarto_on = !quarto_on;
        set_led_red(quarto_on);
        set_led_green(quarto_on);
        red_on = quarto_on;
        green_on = quarto_on;
        controle_manual_led_red = true;  // Desativa modo automático
    }

    // Alterna os LEDs da "sala" (verde + azul)
    if (strstr(req, "GET /sala")) {
        sala_on = !sala_on;
        set_led_green(sala_on);
        set_led_blue(sala_on);
        green_on = sala_on;
        blue_on = sala_on;
    }

    // Alterna os LEDs da "cozinha" (azul + vermelho)
    if (strstr(req, "GET /cozinha")) {
        cozinha_on = !cozinha_on;
        set_led_blue(cozinha_on);
        set_led_red(cozinha_on);
        blue_on = cozinha_on;
        red_on = cozinha_on;
    }

    // Alterna o estado dos sensores (ativar/desativar modo automático)
    if (strstr(req, "GET /sensores")) {
        sensores_ativos = !sensores_ativos;
        if (sensores_ativos) {
            controle_manual_led_red = false;  // Restaura o modo automático
        }
    }
}

// Inicializa o servidor web na porta 80
void iniciar_webserver() {
    struct tcp_pcb *pcb = tcp_new();                  // Cria um novo bloco de controle TCP
    tcp_bind(pcb, IP_ADDR_ANY, 80);                   // Associa à porta 80 (HTTP)
    pcb = tcp_listen(pcb);                            // Coloca em modo de escuta
    tcp_accept(pcb, callback_aceitar);                // Define função para aceitar conexões
}

// Função chamada quando uma nova conexão é aceita
static err_t callback_aceitar(void *arg, struct tcp_pcb *newpcb, err_t err) {
    tcp_recv(newpcb, callback_receber);               // Define função para processar dados recebidos
    return ERR_OK;
}

// Função chamada sempre que há uma nova requisição HTTP
static err_t callback_receber(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err) {
    if (!p) {
        tcp_close(tpcb);                              // Fecha a conexão se não há dados
        return ERR_OK;
    }

    // Copia o conteúdo da requisição HTTP para um buffer manipulável
    char *request = malloc(p->len + 1);
    memcpy(request, p->payload, p->len);
    request[p->len] = 0; // Finaliza a string

    printf("REQ: %s\n", request);                     // Imprime a requisição no console
    processar_requisicao(request);                    // Processa a requisição

    // Gera a resposta HTML com os botões da interface web
    char html[1024];
    snprintf(html, sizeof(html),
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n\r\n"
        "<!DOCTYPE html>\n"
        "<html>\n"
        "<head>\n"
        "<title> Smart Home </title>\n"
        "<style>\n"
        "body { background-color:rgb(1, 57, 70); font-family: Arial, sans-serif; text-align: center; margin-top: 50px; }\n"
        "button { background-color: LightGray; font-size: 32px; margin: 10px 0; padding: 10px 20px; min-width: 300px; border-radius: 10px; }\n"
        ".btn-on  { background-color:#5B3CC4; color: white; }\n"
        ".btn-off { background-color: LightGray; color: black; }\n"
        "h2 { font-size: 64px; margin-bottom: 30px; }\n"
        "</style>\n"
        "</head>\n"
        "<body>\n"
        "<h2>Smart Home - Controle</h2>"
        "<form action=\"./quarto\"><button class=\"%s\">%s Quarto</button></form>\n"
        "<form action=\"./sala\"><button class=\"%s\">%s Sala</button></form>\n"
        "<form action=\"./cozinha\"><button class=\"%s\">%s Cozinha</button></form>\n"
        "<hr>\n"
        "<form action=\"./sensores\"><button class=\"%s\">%s Sensores</button></form>\n"
        "</body></html>",
        quarto_on ? "btn-on" : "btn-off", quarto_on ? "Desligar" : "Ligar",
        sala_on ? "btn-on" : "btn-off", sala_on ? "Desligar" : "Ligar",
        cozinha_on ? "btn-on" : "btn-off", cozinha_on ? "Desligar" : "Ligar",       
        sensores_ativos ? "btn-on" : "btn-off", sensores_ativos ? "Desativar" : "Ativar"
    );

    // Envia a resposta HTML gerada
    tcp_write(tpcb, html, strlen(html), TCP_WRITE_FLAG_COPY);
    tcp_output(tpcb);

    // Libera a memória alocada
    free(request);
    pbuf_free(p);
    return ERR_OK;
}
