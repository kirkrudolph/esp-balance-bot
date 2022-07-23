#include "esp_log.h"
#include "pwm_drivers.h"
#include "driver/ledc.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define TAG "PWM_DRIVERS"

void pwm_task(void){
    
    // Configure Timer
    ledc_timer_config_t timer = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .duty_resolution = LEDC_TIMER_10_BIT,
        .timer_num = LEDC_TIMER_0,
        .freq_hz = 5000,
        .clk_cfg = LEDC_AUTO_CLK
    };

    // Create Timer
    ledc_timer_config(&timer);

    // Configure LEDC
    ledc_channel_config_t channel = {
        .gpio_num = 4,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = LEDC_CHANNEL_0,
        .timer_sel = LEDC_TIMER_0,
        .duty = 0,
        .hpoint = 0
    };
    
    // Create LEDC
    ledc_channel_config(&channel);

    // Sweep duty for LEDC
    for (int i = 0; i < 1024; i++){
      ledc_set_duty_and_update(LEDC_LOW_SPEED_MODE,LEDC_CHANNEL_0,i,0);
      // ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, i);
      // ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
      vTaskDelay(10 / portTICK_PERIOD_MS);
    }

    // Install fade function
    ledc_fade_func_install(0);

    // Fade LED in and out
    while(true)
    {
      ledc_set_fade_time_and_start(LEDC_LOW_SPEED_MODE,LEDC_CHANNEL_0,0,1000,LEDC_FADE_WAIT_DONE);
      ledc_set_fade_time_and_start(LEDC_LOW_SPEED_MODE,LEDC_CHANNEL_0,1024,1000,LEDC_FADE_WAIT_DONE);
    }
}
