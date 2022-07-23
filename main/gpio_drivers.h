#ifndef GPIO_DRIVERS_H 
#define GPIO_DRIVERS_H

#define MOTOR_1_DIRECTION_PIN 2
#define MOTOR_2_DIRECTION_PIN 19

void init_gpio_task(void *params);
void toggle_gpio_task(void *params);

#endif // DRIVERS_H
