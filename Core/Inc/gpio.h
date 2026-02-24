#ifndef GPIO_H_
#define GPIO_H_

#include "stm32f446xx.h"
#include <stdint.h>
#include <stdbool.h>


typedef struct {
    GPIO_TypeDef* port;
    uint16_t pin;
} Led_t;

typedef struct {
    GPIO_TypeDef* port;
    uint16_t pin;
} Button_t;

extern const Led_t red_led, yellow_led, green_led, error_led;
extern const Led_t ped_red_led, ped_green_led;
extern const Button_t button_ped_tl;
extern volatile bool button_is_pressed;
extern volatile uint32_t last_button_press_time;

void led_init(void);

static inline void led_on(const Led_t* led) {
    led->port->BSRR = led->pin;
}

static inline void led_off(const Led_t* led) {
    led->port->BSRR = (uint32_t)led->pin << 16;
}

static inline void led_toggle(const Led_t* led) {
    led->port->ODR ^= led->pin;
}

void button_init(void);
void button_interrupt_init(void);
void EXTI0_IRQHandler(void);

#endif
