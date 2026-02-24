#include "main.h"
#include "stm32f446xx.h"
#include "stm32f4xx_hal_cortex.h"
#include <stdint.h>

#define PSC_VAL 8400UL
#define ARR_VAL 10000UL

volatile uint32_t seconds_counter = 0;

void tim2_init(void) {
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
    (void)RCC->APB1ENR;

    TIM2->PSC = PSC_VAL - 1UL;  // 10kHz
    TIM2->ARR = ARR_VAL - 1UL;  // interrupt each sec
    // enable interrupt
    TIM2->DIER &= ~TIM_DIER_UIE_Msk;
    TIM2->DIER |= TIM_DIER_UIE;
    HAL_NVIC_EnableIRQ(TIM2_IRQn);
    // start
    TIM2->CR1 &= ~TIM_CR1_CEN_Msk;
    TIM2->CR1 |= TIM_CR1_CEN;
}

void TIM2_IRQHandler(void) {
    if(TIM2->SR & TIM_SR_UIF) {
        TIM2->SR &= ~TIM_SR_UIF;
        seconds_counter++;
    }
}