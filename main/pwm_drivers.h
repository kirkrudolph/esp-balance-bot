#ifndef PWM_DRIVERS_H 
#define PWM_DRIVERS_H

#define PWM_1_GPIO_PIN  4               /*  GPIO number for PWM_1               */
#define PWM_2_GPIO_PIN  18              /*  GPIO number for PWM_2               */
#define FADE_TIME_MS    750
#define OFF_DUTY        0   
#define ON_DUTY         1024            // Using LEDC_TIMER_10_BIT

void init_pwm_task(void *params);
void pwm_fade_task(void *params);

#endif // DRIVERS_H
