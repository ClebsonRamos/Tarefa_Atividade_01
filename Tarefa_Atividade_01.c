//-----BIBLIOTECAS NECESSÁRIAS-----
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"

//-----CONSTANTES PARA OS PINOS DO RASPBERRI PI PICO W-----
#define LED_VERMELHO 13
#define LED_AMARELO 12 // Azul no BitDogLab
#define LED_VERDE 11

//-----VARIÁVEIS GLOBAIS-----
uint marcador_status_semaforo = 0;

//-----PROTÓTIPOS-----
void inicializacao_dos_pinos(void);

//-----FUNÇÃO CALLBACK-----
bool repeating_timer_callback(struct repeating_timer *ponteiro){
    // Incremento do marcador do estado do semáforo.
    marcador_status_semaforo++;
    if(marcador_status_semaforo == 3)
        marcador_status_semaforo = 0;
    // Estrutura responsável pelo acionamento dos LEDs.
    switch(marcador_status_semaforo){
        case 0: // Sinal vermelho
            gpio_put(LED_VERMELHO, true);
            gpio_put(LED_AMARELO, false);
            gpio_put(LED_VERDE, false);
            break;
        case 1: // Sinal amarelo (azul no BitDogLab)
            gpio_put(LED_VERMELHO, false);
            gpio_put(LED_AMARELO, true);
            gpio_put(LED_VERDE, false);
            break;
        case 2: // Sinal verde
            gpio_put(LED_VERMELHO, false);
            gpio_put(LED_AMARELO, false);
            gpio_put(LED_VERDE, true);
            break;
    }
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
        switch(marcador_status_semaforo){
            case 0:
                printf("SINAL VERMELHO - Aguarde.\n");
                break;
            case 1:
                printf("SINAL AMARELO - Atencao.\n");
                break;
            case 2:
                printf("SINAL VERDE - Prossiga.\n");
                break;
        }
        sleep_ms(1000);
    }
    return 0;
}

//-----FUNÇÕES COMPLEMENTARES-----
void inicializacao_dos_pinos(void){
    gpio_init(LED_VERMELHO);
    gpio_init(LED_AMARELO);
    gpio_init(LED_VERDE);
    gpio_set_dir(LED_VERMELHO, GPIO_OUT);
    gpio_set_dir(LED_AMARELO, GPIO_OUT);
    gpio_set_dir(LED_VERDE, GPIO_OUT);
}
