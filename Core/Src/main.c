#include "main.h"
#include "cmsis_gcc.h"
#include "gpio.h"
#include "rcc.h"
#include "clock.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_def.h"
#include <stdint.h>
#include <stdbool.h>

typedef enum {CAR_RED_LIGHT, CAR_YELLOW_LIGHT, CAR_GREEN_LIGHT} TrafficLightState;

#define CAR_RED_LIGHT_TIME 10 // seceonds
#define CAR_YELLOW_LIGHT_TIME 4
#define CAR_GREENLIGHT_TIME 20
#define CAR_GREENLIGHT_TIME_BLINK (CAR_GREENLIGHT_TIME - 3)

#define PED_GREENLIGHT_TIME_BLINK (CAR_RED_LIGHT_TIME - 3)

#define PED_BUTTON_THRESHOLD (CAR_GREENLIGHT_TIME >> 1UL)  // devide by 2


static void test_car_led(void) {
    led_toggle(&red_led);
    HAL_Delay(500);
    led_toggle(&yellow_led);
    HAL_Delay(500);
    led_toggle(&green_led);
    HAL_Delay(500);
}

static void test_ped_led(void) {
  led_toggle(&ped_red_led);
  HAL_Delay(500);
  led_toggle(&ped_green_led);
  HAL_Delay(500);
}

void traffic_FSM_process(void) {
  static TrafficLightState current_state = CAR_RED_LIGHT;
  static bool init_traffic = true;
  static uint32_t phase_start_time = 0;
  uint32_t global_time = seconds_counter;
  uint32_t time_in_phase = global_time - phase_start_time;

  if(button_is_pressed) {
    button_is_pressed = false; // reset button flag
    // Has it been 30 seconds since the last button press?
    if((global_time - last_button_press_time) >= 60) {
      // If the light is green now and it has been on for more than half of its time
      if(current_state == CAR_GREEN_LIGHT && time_in_phase >= PED_BUTTON_THRESHOLD) {
        phase_start_time = global_time - (CAR_GREENLIGHT_TIME - 5);
        last_button_press_time = global_time;
      }
    }
  }

  if(init_traffic) {
    led_on(&red_led);
    led_off(&ped_red_led);
    led_on(&ped_green_led);
    init_traffic = false;
    phase_start_time = global_time;
  }
  
  switch (current_state) {  
    case CAR_RED_LIGHT:
      if(time_in_phase >= PED_GREENLIGHT_TIME_BLINK) {
        if ((TIM2->CNT % 5000) < 2500) { // blink led
            led_on(&ped_green_led);
        } else {
            led_off(&ped_green_led);
        }
      }
      if(time_in_phase >= CAR_RED_LIGHT_TIME) {
        led_off(&ped_green_led);
        led_on(&ped_red_led);
        current_state = CAR_YELLOW_LIGHT;
        led_on(&yellow_led);
        phase_start_time = global_time; // when yellow in start
      }
      break;
    case CAR_YELLOW_LIGHT:
      if(time_in_phase < CAR_YELLOW_LIGHT_TIME) break;
      else {
        led_off(&red_led);
        led_off(&yellow_led);
        led_on(&ped_red_led);
        current_state = CAR_GREEN_LIGHT;
        led_on(&green_led);
        phase_start_time = global_time; // when green in start
      }
      break;
    case CAR_GREEN_LIGHT:
      if(time_in_phase >= CAR_GREENLIGHT_TIME_BLINK) {
        if ((TIM2->CNT % 5000) < 2500) {  // blink
            led_on(&green_led);
        } else {
            led_off(&green_led);
        }
      }
      if (time_in_phase >= CAR_GREENLIGHT_TIME){
        led_off(&green_led);
        current_state = CAR_RED_LIGHT;
        led_on(&red_led);
        led_off(&ped_red_led);
        led_on(&ped_green_led);
        phase_start_time = global_time;
      }
      break;
  }
}


int main(void) {
  SystemClock_Config();
  led_init();
  tim2_init();
  button_init();
  button_interrupt_init();

  while (1) {
    traffic_FSM_process();
  }
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  __disable_irq();
  while (1) {
    led_toggle(&error_led);
    HAL_Delay(100);
  }

}