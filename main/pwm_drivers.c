#include "esp_log.h"
#include "pwm_drivers.h"
#include "driver/ledc.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define TAG "PWM_DRIVERS"

void configure_and_install_ledc_timer(void){
    // Configure Timer
    ledc_timer_config_t timer = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .duty_resolution = LEDC_TIMER_10_BIT,
        .timer_num = LEDC_TIMER_0,
        .freq_hz = 5000,
        .clk_cfg = LEDC_AUTO_CLK
    };

    // Create Timer
    ESP_ERROR_CHECK(ledc_timer_config(&timer));
}

void configure_and_install_ledc_channels(void){

    // Configure PWM_1
    ledc_channel_config_t channel_1 = {                 
        .gpio_num = PWM_1_GPIO_PIN,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = LEDC_CHANNEL_0,
        .timer_sel = LEDC_TIMER_0,
        .duty = 0,
        .hpoint = 0
    };

    // Configure PWM_2
    ledc_channel_config_t channel_2 = {
        .gpio_num = PWM_2_GPIO_PIN,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = LEDC_CHANNEL_1,
        .timer_sel = LEDC_TIMER_0,
        .duty = 0,
        .hpoint = 0
    };
    
    // Create LED Control
    ESP_ERROR_CHECK(ledc_channel_config(&channel_1));
    ESP_ERROR_CHECK(ledc_channel_config(&channel_2));
}

void pwm_task(void){
    
    configure_and_install_ledc_timer();                         // TIMER_0
    configure_and_install_ledc_channels();                      // PWM_1, PWM_2

    // Sweep duty for LEDC
    for (int i = 0; i < 1024; i++){
        ledc_set_duty_and_update(LEDC_LOW_SPEED_MODE,LEDC_CHANNEL_0,i,0);
        ledc_set_duty_and_update(LEDC_LOW_SPEED_MODE,LEDC_CHANNEL_1,i,0);

        // ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, i);
        // ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }

    // Install fade function
    ledc_fade_func_install(0);
    //ledc_fade_func_install(1);

    // Fade LED in and out
    while(true)
    {
        ledc_set_fade_time_and_start(LEDC_LOW_SPEED_MODE,LEDC_CHANNEL_0,0,1000,LEDC_FADE_WAIT_DONE);
        //ledc_set_fade_time_and_start(LEDC_LOW_SPEED_MODE,LEDC_CHANNEL_1,0,1000,LEDC_FADE_WAIT_DONE);
        ledc_set_fade_time_and_start(LEDC_LOW_SPEED_MODE,LEDC_CHANNEL_0,1024,1000,LEDC_FADE_WAIT_DONE);
        //ledc_set_fade_time_and_start(LEDC_LOW_SPEED_MODE,LEDC_CHANNEL_1,1024,1000,LEDC_FADE_WAIT_DONE);
    }
    vTaskDelete(NULL);

}
