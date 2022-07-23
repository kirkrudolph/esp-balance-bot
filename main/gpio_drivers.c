#include "driver/gpio.h"
#include "gpio_drivers.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void configure_and_install_gpio_driver(void){
    
    // Configure
    gpio_config_t config;
    config.pin_bit_mask = ((1ULL << MOTOR_1_DIRECTION_PIN) |
                           (1ULL << MOTOR_2_DIRECTION_PIN));    // Configure both at the same time
    config.mode = GPIO_MODE_OUTPUT;
    config.pull_down_en = GPIO_PULLDOWN_ENABLE;                // GPIO_PULLDOWN_ENABLE;
    config.pull_up_en = GPIO_PULLUP_DISABLE;                     // GPIO_PULLUP_DISABLE;
    
    // Install GPIO
    ESP_ERROR_CHECK(gpio_config(&config));
}

void init_gpio_task(void *params){
    
    //configure_and_install_gpio_driver();        // Configure GPIO THIS FUNCTION IS BREAKING EVERYTHING
    gpio_pad_select_gpio(2);
    ESP_ERROR_CHECK(gpio_set_direction(2,GPIO_MODE_OUTPUT));
    vTaskDelete(NULL);
}
void toggle_gpio_task(void *params){
    
    int isOn = 0;
    ESP_ERROR_CHECK(gpio_set_level(MOTOR_1_DIRECTION_PIN,1));
    ESP_ERROR_CHECK(gpio_set_level(MOTOR_2_DIRECTION_PIN,1));

    while(true){
        isOn = !isOn;
        ESP_ERROR_CHECK(gpio_set_level(2,isOn));
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
    
    vTaskDelete(NULL);
}
