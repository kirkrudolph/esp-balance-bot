#include <stdio.h>

#include "esp_log.h"
#include "esp_system.h"
#include "driver/i2c.h"
#include "driver/ledc.h"
#include "driver/gpio.h"
#include "mpu6050.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


#define TAG "MAIN"
#define I2C_MASTER_PORT     I2C_NUM_0       /*  I2C port number for master dev      */
#define I2C_MASTER_SDA_IO   21              /*  gpio number for I2C master data     */
#define I2C_MASTER_SCL_IO   22              /*  gpio number for I2C master clock    */
#define I2C_MASTER_FREQ_HZ  100000          /*  I2C master clock frequency 100kHz   */
#define SLV_RX_BUF_LEN      0
#define SLV_TX_BUF_LEN      0

static mpu6050_handle_t imu = NULL;

void configure_i2c(void){
   
    // Configuration
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
    esp_err_t finish_config = i2c_param_config(I2C_MASTER_PORT, &conf);
    if (finish_config != ESP_OK) ESP_LOGE(TAG,"i2c configure failed!");

    // Install i2c driver
    esp_err_t finish_install = i2c_driver_install(I2C_MASTER_PORT, conf.mode, SLV_RX_BUF_LEN, SLV_TX_BUF_LEN, ESP_INTR_FLAG_LEVEL1);
    if (finish_install != ESP_OK) ESP_LOGE(TAG,"i2c install failed!");
}

void init(void){

    configure_i2c();

    // Create IMU instance
    imu = mpu6050_create(I2C_MASTER_PORT, MPU6050_I2C_ADDRESS);
    if (!imu) ESP_LOGE(TAG,"IMU init failed!");

    /*  DeviceID Options
    //
    // | AD0 | I2C Address |
    // |-----|-------------|
    // |  0  |     104     | 
    // |  1  |     105     |
    */
    uint8_t deviceid;
    esp_err_t device_id_ret = mpu6050_get_deviceid(imu, &deviceid);
    if (device_id_ret != ESP_OK) ESP_LOGE(TAG,"get device id failed!");
    ESP_LOGI(TAG,"IMU I2C DeviceID: %d",deviceid);

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

}

void pwm_task(void){
    
    ledc_timer_config_t timer = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .duty_resolution = LEDC_TIMER_10_BIT,
        .timer_num = LEDC_TIMER_0,
        .freq_hz = 5000,
        .clk_cfg = LEDC_AUTO_CLK
    };

    ledc_timer_config(&timer);

    ledc_channel_config_t channel = {
        .gpio_num = 4,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = LEDC_CHANNEL_0,
        .timer_sel = LEDC_TIMER_0,
        .duty = 0,
        .hpoint = 0
    };
  
    ledc_channel_config(&channel);

    ledc_fade_func_install(0);
    for (int i = 0; i < 1024; i++){
      ledc_set_duty_and_update(LEDC_LOW_SPEED_MODE,LEDC_CHANNEL_0,i,0);
      // ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, i);
      // ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
      vTaskDelay(10 / portTICK_PERIOD_MS);
    }

    while(true)
    {
      ledc_set_fade_time_and_start(LEDC_LOW_SPEED_MODE,LEDC_CHANNEL_0,0,1000,LEDC_FADE_WAIT_DONE);
      ledc_set_fade_time_and_start(LEDC_LOW_SPEED_MODE,LEDC_CHANNEL_0,1024,1000,LEDC_FADE_WAIT_DONE);
    }
}

void read_data_task(void *params){
    
    init();

    while (true){

        // Wake up is necessary to get data.
        esp_err_t wake_up_return = mpu6050_wake_up(imu);
        if (wake_up_return != ESP_OK) ESP_LOGE(TAG,"wake up failed!");

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

void terminate(void){

    // Delete i2c
    esp_err_t check = i2c_driver_delete(I2C_MASTER_PORT);
    if (check != ESP_OK) ESP_LOGE(TAG,"i2c driver delete failed!");

    // Delete IMU
    mpu6050_delete(imu);
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
    //init();
    gpio_config_t GPIO = setup_gpio();
    xTaskCreate(read_data_task, "Read_IMU", 1024 * 8, NULL, 2, NULL);

    //pwm_task();
    

    //terminate();
}
