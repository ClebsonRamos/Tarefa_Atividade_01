#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"

#define LED_VERMELHO 13
#define LED_AZUL 11
#define LED_VERDE 12

//-----VARIÁVEIS GLOBAIS-----
char status_do_semaforo[3] = {'R', 'B', 'G'};
int marcador_status_semaforo = 0;

//-----PROTÓTIPOS-----
void inicializacao_dos_pinos(void);

//-----FUNÇÃO CALLBACK-----
bool repeating_timer_callback(struct repeating_timer *ponteiro){
    switch(status_do_semaforo[marcador_status_semaforo]){
        case 'R': // Sinal vermelho
            gpio_put(LED_VERMELHO, true);
            gpio_put(LED_AZUL, false);
            gpio_put(LED_VERDE, false);
            break;
        case 'B': // Sinal amarelo (azul no BitDogLab)
            gpio_put(LED_VERMELHO, false);
            gpio_put(LED_AZUL, true);
            gpio_put(LED_VERDE, false);
            break;
        case 'G': // Sinal verde
            gpio_put(LED_VERMELHO, false);
            gpio_put(LED_AZUL, false);
            gpio_put(LED_VERDE, true);
            break;
    }
    marcador_status_semaforo++;
    if(marcador_status_semaforo == 3)
        marcador_status_semaforo = 0;
    return true;
}

//-----FUNÇÃO PRINCIPAL-----
int main(void){
    inicializacao_dos_pinos();
    stdio_init_all();

    // Estrutura de temporizador de repetição.
    struct repeating_timer temporizador;

    // Configuração do temporizador para chamar a função de callback a cada 3 segundos.
    add_repeating_timer_ms(3000, repeating_timer_callback, NULL, &temporizador);

    // Acionamento inicial do sinal vermelho.
    gpio_put(LED_VERMELHO, true);

    while(true){
        if(status_do_semaforo[marcador_status_semaforo] == 'r'){
            printf("SINAL VERMELHO - Aguarde.\n");
        }else if(status_do_semaforo[marcador_status_semaforo] == 'b'){
            printf("SINAL AMARELO - Atencao.\n");
        }else if(status_do_semaforo[marcador_status_semaforo] == 'g'){
            printf("SINAL VERDE - Prossiga.\n");
        }
        sleep_ms(1000);
    }
    return 0;
}

//-----FUNÇÕES COMPLEMENTARES-----
void inicializacao_dos_pinos(void){
    gpio_init(LED_VERMELHO);
    gpio_init(LED_AZUL);
    gpio_init(LED_VERDE);
    gpio_set_dir(LED_VERMELHO, GPIO_OUT);
    gpio_set_dir(LED_AZUL, GPIO_OUT);
    gpio_set_dir(LED_VERDE, GPIO_OUT);
}
