#include "main.h"
#include "stm32f446xx.h"
#include "stm32f4xx_hal_cortex.h"
#include "stm32f4xx_hal_gpio.h"
#include <stdint.h>
#include <stdbool.h>
#include "gpio.h"

// Car lights
const Led_t red_led = {GPIOA, GPIO_PIN_10};
const Led_t yellow_led = {GPIOB, GPIO_PIN_5};
const Led_t green_led = {GPIOB, GPIO_PIN_4};
// Pedestrian lights
const Led_t ped_red_led = {GPIOC, GPIO_PIN_0};
const Led_t ped_green_led = {GPIOC, GPIO_PIN_1};
// Debug light
const Led_t error_led = {GPIOA, GPIO_PIN_5};
// Pedestrian button
const Button_t button_ped_tl = {GPIOB, GPIO_PIN_0};
// Button is pressed
volatile bool button_is_pressed = false;

void led_init(void) {
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
    (void)RCC->AHB1ENR;
    red_led.port->MODER &= ~GPIO_MODER_MODER10_Msk;
    red_led.port->MODER |= GPIO_MODER_MODER10_0;

    yellow_led.port->MODER &= ~GPIO_MODER_MODER5_Msk;
    yellow_led.port->MODER |= GPIO_MODER_MODER5_0;

    green_led.port->MODER &= ~GPIO_MODER_MODER4_Msk;
    green_led.port->MODER |= GPIO_MODER_MODER4_0;

    ped_red_led.port->MODER &= ~GPIO_MODER_MODER0_Msk;
    ped_red_led.port->MODER |= GPIO_MODER_MODER0_0;

    ped_green_led.port->MODER &= ~GPIO_MODER_MODER1_Msk;
    ped_green_led.port->MODER |= GPIO_MODER_MODER1_0;

    error_led.port->MODER &= ~GPIO_MODER_MODER5_Msk;
    error_led.port->MODER |= GPIO_MODER_MODER5_0;
}

void button_init(void) {
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
    __asm("nop"); 
    __asm("nop");
    button_ped_tl.port->MODER &= ~GPIO_MODER_MODER0_Msk;
    button_ped_tl.port->PUPDR &= ~GPIO_PUPDR_PUPD0_Msk;
    button_ped_tl.port->PUPDR |= GPIO_PUPDR_PUPDR0_0;
}

void button_interrupt_init(void) {
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
    (void)RCC->APB2ENR;
    SYSCFG->EXTICR[0] &= ~SYSCFG_EXTICR1_EXTI0_Msk;
    SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI0_PB;
    EXTI->IMR &= ~EXTI_IMR_MR0_Msk;
    EXTI->IMR |= EXTI_IMR_IM0;
    EXTI->FTSR &= ~EXTI_FTSR_TR0_Msk;
    EXTI->FTSR |= EXTI_FTSR_TR0;
    HAL_NVIC_EnableIRQ(EXTI0_IRQn);
}

void EXTI0_IRQHandler(void) {
    if(EXTI->PR & EXTI_PR_PR0) {
        EXTI->PR = EXTI_PR_PR0;
        (void)EXTI->PR;
        button_is_pressed = true;
    }
}