#include <stdio.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "i2c_drivers.h"
#include "pwm_drivers.h"

#define TAG "MAIN"

#define GPIO5 5


gpio_config_t setup_gpio(void){
    gpio_config_t config;
    config.pin_bit_mask = ((1ULL<<GPIO5)); //| (1ULL<<12));
    config.mode = GPIO_MODE_OUTPUT;
    config.pull_down_en = GPIO_PULLDOWN_DISABLE;//GPIO_PULLDOWN_ENABLE;
    config.pull_up_en = GPIO_PULLUP_ENABLE;//GPIO_PULLUP_DISABLE;
    gpio_config(&config);

    gpio_set_level(GPIO5,1);


    gpio_pad_select_gpio(2);
    gpio_set_direction(2,GPIO_MODE_OUTPUT);
    int isOn = 0;
    while(true){
        isOn = !isOn;
        gpio_set_level(2,isOn);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }

    return config;
}

void toggle_gpio(gpio_config_t GPIO){
    //esp_err_t gpio_set_level(gpio_num_t gpio_num, uint32_t level)
}

void app_main(void)
{
    xTaskCreate(read_imu_data_task, "Read_IMU", 1024 * 8, NULL, 2, NULL);
    pwm_task();
    setup_gpio();


    //terminate_i2c();
}
