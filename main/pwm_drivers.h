#ifndef PWM_DRIVERS_H 
#define PWM_DRIVERS_H

#define PWM_1_PIN       32              /*  GPIO number for PWM_1               */
#define PWM_2_PIN       14              /*  GPIO number for PWM_2               */
#define FADE_TIME_MS    1000
#define OFF_DUTY        0   
#define ON_DUTY         1024            // Using LEDC_TIMER_10_BIT

void init_pwm_task(void *params);
void pwm_fade_task(void *params);

#endif // DRIVERS_H
