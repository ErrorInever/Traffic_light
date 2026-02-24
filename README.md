## STM32 Traffic Light Controller (FSM Based)
A robust, register-level traffic light control system implemented on the STM32F446RE (Nucleo-64). This project demonstrates a Non-blocking Finite State Machine (FSM) approach to handle car and pedestrian traffic synchronization with real-time button interrupts and rate-limiting logic.

### Features
* Non-blocking Logic: Uses a global timer reference instead of delay() functions, allowing the MCU to remain responsive.
* Finite State Machine (FSM): Clean state transitions (Red -> Yellow -> Green) for both car and pedestrian lanes.
* Interrupt-Driven Button: Pedestrian requests are handled via EXTI (External Interrupt) for immediate response.
* Smart Rate Limiting: "Anti-vandal" logic ensures the pedestrian button only triggers a phase change once every 60 seconds.
* Register-Level Programming: Core peripherals (GPIO, RCC, TIM2, SYSCFG) are configured directly via registers for maximum performance and educational depth.

---

## 🛠 Hardware Configuration

| Component | MCU Pin | Logic | Function |
| :--- | :--- | :--- | :--- |
| **Car Red LED** | PA10 | High = ON | Stop signal for cars |
| **Car Yellow LED** | PB5 | High = ON | Warning signal for cars |
| **Car Green LED** | PB4 | High = ON | Go signal for cars |
| **Pedestrian Red LED** | PC0 | High = ON | Stop signal for pedestrians |
| **Pedestrian Green LED** | PC1 | High = ON | Go signal for pedestrians |
| **Pedestrian Button** | PB0 | Pull-up | Active Low (Interrupt) |

---

## ⏱ Timing Logic

The system operates on a Global Time Reference principle. Instead of halting the processor, we calculate the time elapsed since a state started.

| State | Car Signal | Pedestrian Signal | Duration |
| :--- | :--- | :--- | :--- |
| **CAR_RED_LIGHT** | Red | Green (Blinks at end) | 10 Seconds |
| **CAR_YELLOW_LIGHT** | Yellow | Red | 4 Seconds |
| **CAR_GREEN_LIGHT** | Green | Red | 20 Seconds |

---

## 💻 Code Structure

| File | Purpose |
| :--- | :--- |
| `clock.c / .h` | Timer 2 configuration (Hardware 1Hz tick) |
| `gpio.c / .h` | Pin definitions and EXTI Interrupt Handler |
| `rcc.c / .h` | System clock configuration (84MHz) |
| `main.c` | Main loop with FSM transition logic |

---
