#ifndef GPIO_DRIVERS_H 
#define GPIO_DRIVERS_H

#define MOTOR_1_DIRECTION_1_PIN 33
#define MOTOR_1_DIRECTION_2_PIN 25
#define MOTOR_2_DIRECTION_1_PIN 26
#define MOTOR_2_DIRECTION_2_PIN 27

void init_gpio_task(void *params);
void toggle_gpio_task(void *params);

#endif // GPIO_DRIVERS_H
