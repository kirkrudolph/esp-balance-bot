#include <stdio.h>

#include "esp_log.h"
#include "esp_system.h"
#include "driver/i2c.h"
#include "mpu6050.h"

#define TAG "MAIN"
#define I2C_MASTER_PORT I2C_NUM_0       /*!< I2C port number for master dev */
#define I2C_MASTER_SDA_IO 21            /*!< gpio number for I2C master data  */
#define I2C_MASTER_SCL_IO 22            /*!< gpio number for I2C master clock */
#define I2C_MASTER_FREQ_HZ 100000       /*!< I2C master clock frequency */

void configure_i2c(void){
    // See docs for guidelines on i2c config.

    // Which of the two I2C controllers to use?
    i2c_port_t i2c_master_port = I2C_MASTER_PORT;
    
    // 1. Configuration
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_MASTER_SDA_IO,            // select GPIO specific to your project
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_io_num = I2C_MASTER_SCL_IO,            // select GPIO specific to your project
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_MASTER_FREQ_HZ,     // select frequency specific to your project
        // .clk_flags = 0,                          /*!< Optional, you can use I2C_SCLK_SRC_FLAG_* flags to choose i2c source clock here. */
    };

    // Finish Configuration
    esp_err_t finish_config = i2c_param_config(i2c_master_port, &conf);

    // Error handling
    if (finish_config != ESP_OK) ESP_LOGE(TAG,"i2c configure failed!\n");

    // 2. Install i2c driver
    esp_err_t finish_install = i2c_driver_install(i2c_master_port, conf.mode, 0, 0, ESP_INTR_FLAG_LEVEL1);

    // Error handling
    if (finish_install != ESP_OK) ESP_LOGE(TAG,"i2c install failed!\n");

}

mpu6050_handle_t init(void){

    configure_i2c();

    mpu6050_handle_t imu = mpu6050_create(I2C_MASTER_PORT, MPU6050_I2C_ADDRESS);

    if (!imu) ESP_LOGE(TAG,"IMU init failed!\n");

    return imu;
}

void read_data(void){
}

void terminate(mpu6050_handle_t imu){

    // Delete i2c
    esp_err_t check = i2c_driver_delete(I2C_MASTER_PORT);
    if (check != ESP_OK) ESP_LOGE(TAG,"i2c driver delete failed!\n");

    // Delete IMU
    mpu6050_delete(imu);
}

void app_main(void)
{
    mpu6050_handle_t imu = init();

    // Wake up is necessary to get data.
    esp_err_t wake_up_return = mpu6050_wake_up(imu);
    if (wake_up_return != ESP_OK) ESP_LOGE(TAG,"wake up failed!\n");

    // Test "get_deviceid"
    uint8_t deviceid;
    esp_err_t test = mpu6050_get_deviceid(imu, &deviceid);
    if (test != ESP_OK) ESP_LOGE(TAG,"get device id failed!\n");

    // Test "get_raw_acce"
    mpu6050_raw_acce_value_t raw_acce_value;
    esp_err_t test2 = mpu6050_get_raw_acce(imu, &raw_acce_value);
    if (test2 != ESP_OK) ESP_LOGE(TAG,"get raw accel failed!\n");

    // Test "get_acce"
    mpu6050_acce_value_t acce_value;
    esp_err_t test3 = mpu6050_get_acce(imu, &acce_value);
    if (test3 != ESP_OK) ESP_LOGE(TAG,"get accel failed!\n");

    // Print out tests
    printf("%d\n",deviceid);
    printf("x: %d, y: %d,z: %d \n",raw_acce_value.raw_acce_x,raw_acce_value.raw_acce_y,raw_acce_value.raw_acce_z);
    printf("x: %f, y: %f,z: %f \n",acce_value.acce_x,acce_value.acce_y,acce_value.acce_z);

    // Clean up resources
    terminate(imu);
}
