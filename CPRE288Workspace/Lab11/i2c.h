/*
 * @file i2c.h
 * @brief Fundamental functions for using I2C with TM4C
 * @author Michael Farmer
 * @date 4/18/2026
 */

#ifndef I2C_H_
#define I2C_H_

#include <inc/tm4c123gh6pm.h>
#include <stdbool.h>
#include <stdint.h>

void i2c_init();
void set_slave_address(char addr);

void i2c_imu_send_addr(char data);
void i2c_send_bytes(char* data, uint8_t size);
uint8_t i2c_imu_read_register(char address);
void i2c_imu_read_registers(uint8_t* return_arr, uint8_t num_bytes, uint8_t start_addr);
void i2c_send_byte(char data);


#endif /* I2C_H_ */
