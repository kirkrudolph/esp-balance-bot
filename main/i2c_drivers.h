#ifndef I2C_DRIVERS_H 
#define I2C_DRIVERS_H

#include "mpu6050.h"

// I2C
void configure_and_install_i2c_driver(void);
void terminate_i2c_driver(void);

// IMU - MPU6050
mpu6050_handle_t imu_init(void);
void terminate_imu_instance(void);
void get_mpu6050_device_id(mpu6050_handle_t sensor);


#endif // I2C_DRIVERS_H


