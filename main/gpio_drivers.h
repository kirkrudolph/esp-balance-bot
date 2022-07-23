#ifndef GPIO_DRIVERS_H 
#define GPIO_DRIVERS_H

#define MOTOR_1_DIRECTION 5
#define MOTOR_2_DIRECTION 6

void init_gpio_task(void *params);
void toggle_gpio_task(void *params);

#endif // DRIVERS_H
