/*
 * SCD41.c
 *
 *  Created on: Jan 25, 2022
 *      Author: hunte
 */

#include "main.h"
#include "SCD41.h"
#include <stdio.h>

HAL_StatusTypeDef getAutomaticSelfCalibration(I2C_HandleTypeDef *hi2x, uint8_t *enabled) {
	HAL_StatusTypeDef ret;
	uint8_t buffer[3];

	// FAIRLY CONFIDENT THIS IS WHAT CAUSES ERROR
	// Set buffer contents to getAutomaticSelfCalibration
	buffer[2] = 0x00;
	buffer[1] = 0x23;
	buffer[0] = 0x13;

	// Send command to SCD41
	// ERROR: waiting until BTF flag is set - stm32f4xx_hal_i2c.c line 1114
	ret = HAL_I2C_Master_Transmit(hi2x, (SCD41_I2C_ADDRESS<<1), buffer, sizeof(buffer), 100);
	if (ret != HAL_OK) {
		return ret;
	}

	// Read result of command from SCD41
	ret = HAL_I2C_Master_Receive(hi2x, (SCD41_I2C_ADDRESS<<1) | (0x01), buffer, sizeof(buffer), 100);
	if (ret != HAL_OK) {
		return ret;
	}

	enabled = &buffer[0];
	return HAL_OK;
}

HAL_StatusTypeDef startPeriodicMeasurment(I2C_HandleTypeDef *hi2x) {
	HAL_StatusTypeDef ret;
	uint8_t buffer[2];

	// FAIRLY CONFIDENT THIS IS WHAT CAUSES ERROR
	// Set buffer contents to startPeriodicMeasurment: 0x21b1
	buffer[1] = 0x21;
	buffer[0] = 0xb1;

	// Send command to SCD41
	// ERROR: waiting until BTF flag is set - stm32f4xx_hal_i2c.c line 1114
	ret = HAL_I2C_Master_Transmit(hi2x, (SCD41_I2C_ADDRESS<<1), buffer, sizeof(buffer), 100);
	if (ret != HAL_OK) {
		return ret;
	}

	return HAL_OK;
}

HAL_StatusTypeDef readMeasurment(I2C_HandleTypeDef *hi2x, uint16_t * co2, uint16_t *temp, uint16_t *humidity) {
	HAL_StatusTypeDef ret;
	uint8_t input_buffer[2];
	uint8_t output_buffer[9];

	// FAIRLY CONFIDENT THIS IS WHAT CAUSES ERROR
	// Set buffer contents to readMeasurment: 0xec05
	input_buffer[1] = 0xec;
	input_buffer[0] = 0x05;

	// Send command to SCD41
	// ERROR: waiting until BTF flag is set - stm32f4xx_hal_i2c.c line 1114
	ret = HAL_I2C_Master_Transmit(hi2x, (SCD41_I2C_ADDRESS<<1), input_buffer, sizeof(input_buffer), 100);
	if (ret != HAL_OK) {
		return ret;
	}
	// Read result of command from SCD41
	ret = HAL_I2C_Master_Receive(hi2x, (SCD41_I2C_ADDRESS<<1) | (0x01), output_buffer, sizeof(output_buffer), 100);
	if (ret != HAL_OK) {
		return ret;
	}

	uint16_t co2_v = (output_buffer[0]<<8 | output_buffer[1]);
	uint16_t temp_v = (output_buffer[3]<<8 | output_buffer[4]);
	uint16_t humidity_v = (output_buffer[6]<<8 | output_buffer[7]);
	co2 = &co2_v;
	temp = &temp_v;
	humidity = &humidity_v;

	return HAL_OK;
}



