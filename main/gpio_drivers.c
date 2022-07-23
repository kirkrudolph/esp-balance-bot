#include "driver/gpio.h"
#include "gpio_drivers.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void configure_and_install_gpio_driver(void){
    
    // Configure
    gpio_config_t config;
    config.pin_bit_mask = ( (1ULL << MOTOR_1_DIRECTION_1_PIN) |
                            (1ULL << MOTOR_1_DIRECTION_2_PIN) |
                            (1ULL << MOTOR_2_DIRECTION_1_PIN) |
                            (1ULL << MOTOR_2_DIRECTION_2_PIN));     // Configure all GPIO at the same time
    config.mode         = GPIO_MODE_OUTPUT;                         // GPIO are outputs
    config.pull_down_en = GPIO_PULLDOWN_ENABLE;                     // GPIO_PULLDOWN_ENABLE;
    config.pull_up_en   = GPIO_PULLUP_DISABLE;                      // GPIO_PULLUP_DISABLE;
    
    // Install GPIO
    ESP_ERROR_CHECK(gpio_config(&config));
}

void init_gpio_task(void *params){
    
    configure_and_install_gpio_driver();
    vTaskDelete(NULL);
}

void toggle_gpio_task(void *params){
    
    int isOn = 0;

    while(true){
        isOn = !isOn;
        ESP_ERROR_CHECK( gpio_set_level( MOTOR_1_DIRECTION_1_PIN, 1));
        ESP_ERROR_CHECK( gpio_set_level( MOTOR_1_DIRECTION_2_PIN,  0));
        ESP_ERROR_CHECK( gpio_set_level( MOTOR_2_DIRECTION_1_PIN, 0));
        ESP_ERROR_CHECK( gpio_set_level( MOTOR_2_DIRECTION_2_PIN,  1));
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
    vTaskDelete(NULL);
}
