#include "esp_log.h"
#include "driver/i2c.h"
#include "i2c_drivers.h"


#define TAG "I2C_DRIVERS"

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

void print_imu_device_id(mpu6050_handle_t imu){

    /*  MPU6050 DeviceID Options
    //
    // | AD0 | I2C Address |
    // |-----|-------------|
    // |  0  |     104     | 
    // |  1  |     105     |
    */
    uint8_t deviceid;
    ESP_ERROR_CHECK(mpu6050_get_deviceid(imu, &deviceid));
    ESP_LOGI(TAG,"IMU I2C DeviceID: %d",deviceid);
}

mpu6050_handle_t imu_init(void){

    // Create IMU instance
    imu = mpu6050_create(I2C_MASTER_PORT, MPU6050_I2C_ADDRESS);
    if (!imu) ESP_LOGE(TAG,"IMU init failed!");
    return imu;
}

void print_imu_sensitivity(mpu6050_handle_t imu){
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
    ESP_ERROR_CHECK(mpu6050_get_acce_sensitivity(imu, &acce_sensitivity));
    ESP_ERROR_CHECK(mpu6050_get_gyro_sensitivity(imu, &gyro_sensitivity));
    ESP_LOGI(TAG,"Accel/Gyro Sensitivity: %f, %f", acce_sensitivity, gyro_sensitivity);
}

void print_imu_raw_data(mpu6050_handle_t imu){
    mpu6050_raw_acce_value_t raw_acce_value;
    ESP_ERROR_CHECK(mpu6050_get_raw_acce(imu, &raw_acce_value));
    ESP_LOGI(TAG, "a_raw_x: %d, a_raw_y: %d, a_raw_z: %d",raw_acce_value.raw_acce_x,raw_acce_value.raw_acce_y,raw_acce_value.raw_acce_z);
    
    mpu6050_raw_gyro_value_t raw_gyro_value;
    ESP_ERROR_CHECK(mpu6050_get_raw_gyro(imu, &raw_gyro_value));
    ESP_LOGI(TAG, "g_raw_x: %d, g_raw_y: %d, g_raw_z: %d",raw_gyro_value.raw_gyro_x,raw_gyro_value.raw_gyro_y,raw_gyro_value.raw_gyro_z);
}

void print_imu_scaled_data(mpu6050_handle_t imu){
    mpu6050_acce_value_t acce_value;
    ESP_ERROR_CHECK(mpu6050_get_acce(imu, &acce_value));
    ESP_LOGI(TAG,"a_x: %f, a_y: %f, a_z: %f",acce_value.acce_x,acce_value.acce_y,acce_value.acce_z);
    
    mpu6050_gyro_value_t gyro_value;
    ESP_ERROR_CHECK(mpu6050_get_gyro(imu, &gyro_value));
    ESP_LOGI(TAG,"g_x: %f, g_y: %f, g_z: %f",gyro_value.gyro_x,gyro_value.gyro_y,gyro_value.gyro_z);
}

void print_imu_filter_data(mpu6050_handle_t imu){
    mpu6050_acce_value_t acce_value;
    mpu6050_gyro_value_t gyro_value;
    complimentary_angle_t complimentary_angle;
    ESP_ERROR_CHECK(mpu6050_complimentory_filter(imu, &acce_value, &gyro_value, &complimentary_angle));
    ESP_LOGI(TAG,"roll: %f, pitch: %f\n",complimentary_angle.roll,complimentary_angle.pitch);
}

void set_imu_sensitivity(mpu6050_handle_t imu){
    ESP_ERROR_CHECK(mpu6050_config(imu, ACCE_FS_4G, GYRO_FS_500DPS));
}

void read_imu_data_task(void *params){
    
    configure_and_install_i2c_driver();         // Install i2c driver
    mpu6050_handle_t imu = imu_init();          // Create IMU instance

    while (true){

        ESP_ERROR_CHECK(mpu6050_wake_up(imu));  // Wake up is necessary to get data.

        print_imu_raw_data(imu);                // Print Raw Data
        print_imu_scaled_data(imu);             // Print Scaled Data
        print_imu_filter_data(imu);             // Print Filter Data
        
        ESP_ERROR_CHECK(mpu6050_sleep(imu));    // Sleep after reading data.
        
        vTaskDelay(pdMS_TO_TICKS(2000));        // Delay 1s
    }
    vTaskDelete(NULL);
}

void terminate_i2c_driver(void){
    ESP_ERROR_CHECK(i2c_driver_delete(I2C_MASTER_PORT));
}

void terminate_imu_instance(void){
    mpu6050_delete(imu);
}