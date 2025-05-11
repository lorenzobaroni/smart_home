#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lwip/tcp.h"
#include "leds.h"
#include "sensores.h"
#include <stdbool.h>

// Variáveis globais de estado dos LEDs
bool red_on = false;
bool green_on = false;
bool blue_on = false;

static err_t callback_aceitar(void *arg, struct tcp_pcb *newpcb, err_t err);
static err_t callback_receber(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err);

void processar_requisicao(char *req) {
    if (strstr(req, "GET /vermelho")) {
    red_on = !red_on;
    set_led_red(red_on);
    controle_manual_led_red = true;
    }
    if (strstr(req, "GET /verde")) {
    green_on = !green_on;
    set_led_green(green_on);
    }
    if (strstr(req, "GET /azul")) {
    blue_on = !blue_on;
    set_led_blue(blue_on);
    }
    if (strstr(req, "GET /sensor_presenca")) {
        sensor_presenca_ativo = !sensor_presenca_ativo;
    }
    if (strstr(req, "GET /sensor_luz")) {
        sensor_luz_ativo = !sensor_luz_ativo;
    }
}

void iniciar_webserver() {
    struct tcp_pcb *pcb = tcp_new();
    tcp_bind(pcb, IP_ADDR_ANY, 80);
    pcb = tcp_listen(pcb);
    tcp_accept(pcb, callback_aceitar);
}

static err_t callback_aceitar(void *arg, struct tcp_pcb *newpcb, err_t err) {
    tcp_recv(newpcb, callback_receber);
    return ERR_OK;
}

static err_t callback_receber(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err) {
    if (!p) {
        tcp_close(tpcb);
        return ERR_OK;
    }

    char *request = malloc(p->len + 1);
    memcpy(request, p->payload, p->len);
    request[p->len] = 0;

    printf("REQ: %s\n", request);
    processar_requisicao(request);

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
        "<form action=\"./vermelho\"><button class=\"%s\">%s Vermelho</button></form>\n"
        "<form action=\"./verde\"><button class=\"%s\">%s Verde</button></form>\n"
        "<form action=\"./azul\"><button class=\"%s\">%s Azul</button></form>\n"
        "<hr>\n"
        "<form action=\"./sensor_presenca\"><button class=\"%s\">%s Presenca</button></form>\n"
        "<form action=\"./sensor_luz\"><button class=\"%s\">%s LDR</form>\n"
        "</body></html>",
        red_on ? "btn-on" : "btn-off",
        red_on ? "Desligar" : "Ligar",
        green_on ? "btn-on" : "btn-off",
        green_on ? "Desligar" : "Ligar",
        blue_on ? "btn-on" : "btn-off",
        blue_on ? "Desligar" : "Ligar",
        sensor_presenca_ativo ? "btn-on" : "btn-off",
        sensor_presenca_ativo ? "Desativar" : "Ativar",
        sensor_luz_ativo ? "btn-on" : "btn-off",
        sensor_luz_ativo ? "Desativar" : "Ativar"
    );

    tcp_write(tpcb, html, strlen(html), TCP_WRITE_FLAG_COPY);
    tcp_output(tpcb);
    free(request);
    pbuf_free(p);
    return ERR_OK;
}
