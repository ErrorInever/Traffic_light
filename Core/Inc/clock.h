#ifndef CLOCK_H_
#define CLOCK_H_

extern volatile uint32_t seconds_counter;

void tim2_init(void);
void TIM2_IRQHandler(void);

#endif
