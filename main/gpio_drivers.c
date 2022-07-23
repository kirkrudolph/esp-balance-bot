#include "driver/gpio.h"
#include "gpio_drivers.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void configure_and_install_gpio_driver(void){
    
    // Configure
    gpio_config_t config;
    config.pin_bit_mask = ((1ULL << MOTOR_1_DIRECTION) |
                           (1ULL << MOTOR_2_DIRECTION));        // Configure both at the same time
    config.mode = GPIO_MODE_OUTPUT;
    config.pull_down_en = GPIO_PULLDOWN_DISABLE;                // GPIO_PULLDOWN_ENABLE;
    config.pull_up_en = GPIO_PULLUP_ENABLE;                     // GPIO_PULLUP_DISABLE;
    
    // Install GPIO
    gpio_config(&config);
}

void init_gpio_task(void *params){
    
    //configure_and_install_gpio_driver();        // Configure GPIO THIS FUNCTION IS BREAKING EVERYTHING
    gpio_pad_select_gpio(2);
    gpio_set_direction(2,GPIO_MODE_OUTPUT);
    vTaskDelete(NULL);

}
void toggle_gpio_task(void *params){
    
    int isOn = 0;

    while(true){
        isOn = !isOn;
        gpio_set_level(2,isOn);
        gpio_set_level(MOTOR_1_DIRECTION,1);
        gpio_set_level(MOTOR_2_DIRECTION,1);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
    
    vTaskDelete(NULL);
}
