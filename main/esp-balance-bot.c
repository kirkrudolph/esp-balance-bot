#include <stdio.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "i2c_drivers.h"
#include "pwm_drivers.h"
#include "gpio_drivers.h"

#define TAG "MAIN"

// -----------------------------------------
//             Balance Bot Pinout         
// -----------------------------------------
// | USE                      | FUNC | PIN |
// |--------------------------|------|-----|
// | MOTOR_1_DIRECTION_PIN    | GPIO | 5   |
// | MOTOR_2_DIRECTION_PIN    | GPIO | 6   |
// | I2C_MASTER_SDA_PIN       | I2C  | 21  |
// | I2C_MASTER_SCL_PIN       | I2C  | 22  |
// | PWM_1_PIN                | PWM  | 4   |
// | PWM_2_PIN                | PWM  | 18  |
// -----------------------------------------

void app_main(void)
{
    // Init
    BaseType_t xReturned_gpio_init = xTaskCreate( &init_gpio_task, "Init_GPIO", 1024 * 4, NULL, 2, NULL);
    BaseType_t xReturned_pwm_init  = xTaskCreate(  &init_pwm_task,  "Init_PWM", 1024 * 4, NULL, 2, NULL);
    vTaskDelay(pdMS_TO_TICKS(500));
    
    // App
    BaseType_t xReturned_gpio = xTaskCreate(   &toggle_gpio_task, "Toggle_GPIO", 1024 * 4, NULL, 2, NULL);
    BaseType_t xReturned_imu  = xTaskCreate( &read_imu_data_task,    "Read_IMU", 1024 * 4, NULL, 2, NULL);
    BaseType_t xReturned_pwm  = xTaskCreate(      &pwm_fade_task,         "PWM", 1024 * 4, NULL, 2, NULL);

    // Error Handling
    if (xReturned_gpio_init != pdPASS)  ESP_LOGE(TAG,"GPIO INIT: COULD_NOT_ALLOCATE_REQUIRED_MEMORY");
    if (xReturned_pwm_init  != pdPASS)  ESP_LOGE(TAG,"PWM INIT: COULD_NOT_ALLOCATE_REQUIRED_MEMORY");
    if (xReturned_imu       != pdPASS)  ESP_LOGE(TAG,"IMU: COULD_NOT_ALLOCATE_REQUIRED_MEMORY");
    if (xReturned_gpio      != pdPASS)  ESP_LOGE(TAG,"GPIO: COULD_NOT_ALLOCATE_REQUIRED_MEMORY");
    if (xReturned_pwm       != pdPASS)  ESP_LOGE(TAG,"PWM: COULD_NOT_ALLOCATE_REQUIRED_MEMORY");
}
