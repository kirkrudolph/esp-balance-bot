#include <stdio.h>
#include "esp_log.h"
#include "esp_system.h"
#include "mpu6050.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "i2c_drivers.h"
#include "pwm_drivers.h"

#define TAG "MAIN"


void read_data_task(void *params){
    
    configure_and_install_i2c_driver();         // Install i2c driver
    mpu6050_handle_t imu = imu_init();          // Create IMU instance

    while (true){

        // Wake up is necessary to get data.
        ESP_ERROR_CHECK(mpu6050_wake_up(imu));

        // Test "get_raw"
/* Raw data isn't very useful.
        mpu6050_raw_acce_value_t raw_acce_value;
        esp_err_t raw_acce_result = mpu6050_get_raw_acce(imu, &raw_acce_value);
        if (raw_acce_result != ESP_OK) ESP_LOGE(TAG,"get raw accel failed!");
        ESP_LOGI(TAG, "a_raw_x: %d, a_raw_y: %d, a_raw_z: %d",raw_acce_value.raw_acce_x,raw_acce_value.raw_acce_y,raw_acce_value.raw_acce_z);

        mpu6050_raw_gyro_value_t raw_gyro_value;
        esp_err_t raw_gyro_result = mpu6050_get_raw_gyro(imu, &raw_gyro_value);
        if (raw_gyro_result != ESP_OK) ESP_LOGE(TAG,"get raw gyro failed!");
        ESP_LOGI(TAG, "g_raw_x: %d, g_raw_y: %d, g_raw_z: %d",raw_gyro_value.raw_gyro_x,raw_gyro_value.raw_gyro_y,raw_gyro_value.raw_gyro_z);
*/

        // Test "get_value"
        mpu6050_acce_value_t acce_value;
        esp_err_t acce_result = mpu6050_get_acce(imu, &acce_value);
        if (acce_result != ESP_OK) ESP_LOGE(TAG,"get accel failed!");
        ESP_LOGI(TAG,"a_x: %f, a_y: %f, a_z: %f",acce_value.acce_x,acce_value.acce_y,acce_value.acce_z);

        mpu6050_gyro_value_t gyro_value;
        esp_err_t gyro_result = mpu6050_get_gyro(imu, &gyro_value);
        if (gyro_result != ESP_OK) ESP_LOGE(TAG,"get gyro failed!");
        ESP_LOGI(TAG,"g_x: %f, g_y: %f, g_z: %f",gyro_value.gyro_x,gyro_value.gyro_y,gyro_value.gyro_z);

        // Test "filter"
        complimentary_angle_t complimentary_angle;
        esp_err_t filter_results = mpu6050_complimentory_filter(imu, &acce_value, &gyro_value, &complimentary_angle);
        if (filter_results != ESP_OK) ESP_LOGE(TAG,"complimentory filter failed!");
        ESP_LOGI(TAG,"roll: %f, pitch: %f\n",complimentary_angle.roll,complimentary_angle.pitch);

        // Sleep after data has been read.
        esp_err_t sleep_return = mpu6050_sleep(imu);
        if (sleep_return != ESP_OK) ESP_LOGE(TAG,"sleep failed!");

        // Delay 1s
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}



gpio_config_t setup_gpio(void){
    gpio_config_t config;
    config.pin_bit_mask = ((1ULL<<4)); //| (1ULL<<12));
    config.mode = GPIO_MODE_OUTPUT;
    config.pull_down_en = GPIO_PULLDOWN_DISABLE;//GPIO_PULLDOWN_ENABLE;
    config.pull_up_en = GPIO_PULLUP_ENABLE;//GPIO_PULLUP_DISABLE;
    gpio_config(&config);

    gpio_set_level(4,1);


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
    xTaskCreate(read_data_task, "Read_IMU", 1024 * 8, NULL, 2, NULL);
    setup_gpio();

    //pwm_task();

    //terminate_i2c();
}
