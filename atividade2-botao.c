#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"

#define PIN_LED_AZUL 11
#define PIN_LED_VERMELHO 12
#define PIN_LED_VERDE 13
#define PIN_BOTAO 5

volatile bool sequencia_led_ativa = false;

// Callback para apagar o LED verde
int64_t desligar_verde_callback(alarm_id_t id, void *user_data) {
    gpio_put(PIN_LED_VERDE, 0);
    sequencia_led_ativa = false;
    return 0;
}

// Callback para apagar o LED vermelho
int64_t desligar_vermelho_callback(alarm_id_t id, void *user_data) {
    gpio_put(PIN_LED_VERMELHO, 0);
    add_alarm_in_ms(3000, desligar_verde_callback, NULL, false);
    return 0;
}

// Callback para apagar o LED azul
int64_t desligar_azul_callback(alarm_id_t id, void *user_data) {
    gpio_put(PIN_LED_AZUL, 0);
    add_alarm_in_ms(3000, desligar_vermelho_callback, NULL, false);
    return 0;
}

// Callback para ativar todos os LEDs
void botao_callback(uint gpio, uint32_t events) {
    if (!sequencia_led_ativa) {
        sequencia_led_ativa = true;
        gpio_put(PIN_LED_AZUL, 1);
        gpio_put(PIN_LED_VERMELHO, 1);
        gpio_put(PIN_LED_VERDE, 1);
        add_alarm_in_ms(3000, desligar_azul_callback, NULL, false);
    }
}

// Função para debounce do botão
bool debounce_botao() {
    sleep_ms(50);
    return gpio_get(PIN_BOTAO) == 0;
}

int main() {

    gpio_init(PIN_LED_AZUL);
    gpio_set_dir(PIN_LED_AZUL, GPIO_OUT);
    gpio_init(PIN_LED_VERMELHO);
    gpio_set_dir(PIN_LED_VERMELHO, GPIO_OUT);
    gpio_init(PIN_LED_VERDE);
    gpio_set_dir(PIN_LED_VERDE, GPIO_OUT);

    gpio_init(PIN_BOTAO);
    gpio_set_dir(PIN_BOTAO, GPIO_IN);
    gpio_pull_up(PIN_BOTAO);

    gpio_set_irq_enabled_with_callback(PIN_BOTAO, GPIO_IRQ_EDGE_FALL, true, &botao_callback);

    while (true) {
        sleep_ms(100);
    }
}
