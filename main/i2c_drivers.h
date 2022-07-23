#ifndef I2C_DRIVERS_H 
#define I2C_DRIVERS_H

#include "mpu6050.h"

// I2C Configuration
#define I2C_MASTER_PORT     I2C_NUM_0       /*  I2C port number for master dev      */
#define I2C_MASTER_SDA_PIN  21              /*  gpio number for I2C master data     */
#define I2C_MASTER_SCL_PIN  22              /*  gpio number for I2C master clock    */
#define I2C_MASTER_FREQ_HZ  100000          /*  I2C master clock frequency 100kHz   */
#define I2C_SLV_RX_BUF_LEN  0               /*  I2C Slave Recieve Buffer            */
#define I2C_SLV_TX_BUF_LEN  0               /*  I2C Slave Transfer Buffer           */

// I2C FUNCTIONS
void configure_and_install_i2c_driver(void);
void terminate_i2c_driver(void);

// IMU - MPU6050
mpu6050_handle_t imu_init(void);
void print_imu_device_id(mpu6050_handle_t imu);
void print_imu_sensitivity(mpu6050_handle_t imu);
void print_imu_raw_data(mpu6050_handle_t imu);
void print_imu_scaled_data(mpu6050_handle_t imu);
void print_imu_filter_data(mpu6050_handle_t imu);
void set_imu_sensitivity(mpu6050_handle_t imu);
void read_imu_data_task(void *params);
void terminate_imu_instance(void);


#endif // I2C_DRIVERS_H


