#include "esp_log.h"
#include "i2c_drivers.h"
#include "driver/i2c.h"
#include "driver/gpio.h"

#define TAG "I2C_DRIVERS"

#define I2C_MASTER_PORT     I2C_NUM_0       /*  I2C port number for master dev      */
#define I2C_MASTER_SDA_IO   21              /*  gpio number for I2C master data     */
#define I2C_MASTER_SCL_IO   22              /*  gpio number for I2C master clock    */
#define I2C_MASTER_FREQ_HZ  100000          /*  I2C master clock frequency 100kHz   */
#define I2C_SLV_RX_BUF_LEN  0               /*  I2C Slave Recieve Buffer            */
#define I2C_SLV_TX_BUF_LEN  0               /*  I2C Slave Transfer Buffer           */

static mpu6050_handle_t imu = NULL;

void configure_and_install_i2c_driver(void){

    // I2C Configuration
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,                    // ESP32 is the master
        .sda_io_num = I2C_MASTER_SDA_IO,            // select GPIO specific to your project
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_io_num = I2C_MASTER_SCL_IO,            // select GPIO specific to your project
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_MASTER_FREQ_HZ,     // select frequency specific to your project
        // .clk_flags = 0,                          /*!< Optional, you can use I2C_SCLK_SRC_FLAG_* flags to choose i2c source clock here. */
    };

    // Finish Configuration
    ESP_ERROR_CHECK(i2c_param_config(I2C_MASTER_PORT, &conf));

    // Install i2c driver
    ESP_ERROR_CHECK(i2c_driver_install(I2C_MASTER_PORT, conf.mode, I2C_SLV_RX_BUF_LEN, I2C_SLV_TX_BUF_LEN, ESP_INTR_FLAG_LEVEL1));
}

void get_mpu6050_device_id(mpu6050_handle_t sensor){

    /*  MPU6050 DeviceID Options
    //
    // | AD0 | I2C Address |
    // |-----|-------------|
    // |  0  |     104     | 
    // |  1  |     105     |
    */
    uint8_t deviceid;
    ESP_ERROR_CHECK(mpu6050_get_deviceid(sensor, &deviceid));
    ESP_LOGI(TAG,"IMU I2C DeviceID: %d",deviceid);
}

mpu6050_handle_t imu_init(void){

    // Create IMU instance
    imu = mpu6050_create(I2C_MASTER_PORT, MPU6050_I2C_ADDRESS);
    if (!imu) ESP_LOGE(TAG,"IMU init failed!");

    /* Sensitivity Options
    //
    // |     Setting      |  Value  | Range (g)| Resolution (g/bit) |
    // |------------------|---------|----------|--------------------|
    // | ACCE_FS_2G       |    0    |   ±  2   |  1/16,384 = 1/2^14 |
    // | ACCE_FS_4G       |    1    |   ±  4   |   1/8,192 = 1/2^13 |
    // | ACCE_FS_8G       |    2    |   ±  8   |   1/4,096 = 1/2^12 |
    // | ACCE_FS_16G      |    3    |   ± 16   |   1/2,048 = 1/2^11 |
    //
    // |     Setting      |  Value  | Range (deg/s)| Resolution (deg/s/bit) |
    // |------------------|---------|--------------|------------------------|
    // | GYRO_FS_250DPS   |    0    |    ±  250    |   1/131  = 0.00763     |
    // | GYRO_FS_500DPS   |    1    |    ±  500    |   1/65.5 = 0.01527     |
    // | GYRO_FS_1000DPS  |    2    |    ± 1000    |   1/32.8 = 0.03053     |
    // | GYRO_FS_2000DPS  |    3    |    ± 2000    |   1/16.4 = 0.06107     |
    */

    // Check Sensitivities
    float acce_sensitivity;
    float gyro_sensitivity;
    esp_err_t acce_sens_ret = mpu6050_get_acce_sensitivity(imu, &acce_sensitivity);
    if (acce_sens_ret != ESP_OK) ESP_LOGE(TAG,"Couldn't get Accel Sensitivity!");
    esp_err_t gyro_sens_ret = mpu6050_get_gyro_sensitivity(imu, &gyro_sensitivity);
    if (gyro_sens_ret != ESP_OK) ESP_LOGE(TAG,"Couldn't get Gyro Sensitivity!");
    ESP_LOGI(TAG,"Accel/Gyro Sensitivity: %f, %f", acce_sensitivity, gyro_sensitivity);

    if (0)
    {
        esp_err_t config_error = mpu6050_config(imu, ACCE_FS_4G, GYRO_FS_500DPS);
        if (config_error != ESP_OK) ESP_LOGE(TAG,"Couldn't set Accel/Gyro Sensitivity!");
    }

    return imu;
}

void terminate_i2c_driver(void){
    // Delete i2c
    ESP_ERROR_CHECK(i2c_driver_delete(I2C_MASTER_PORT));
}

void terminate_imu_instance(void){
    // Delete IMU
    mpu6050_delete(imu);
}