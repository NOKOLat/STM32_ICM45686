/*
 * ICM45686_HAL_I2C.cpp
 *
 *  Created on: Mar 15, 2025
 *      Author: Sezakiaoi
 */

#include <ICM45686_HAL_I2C.h>

ICM45686_HAL_I2C::ICM45686_HAL_I2C(I2C_HandleTypeDef* I2cPin) {

	this->I2cPin = I2cPin;
}

void ICM45686_HAL_I2C::Write(ICM45686::REGISTER REGISTER_ADDR, uint8_t* TxBuffer, uint8_t Len){

	HAL_I2C_Mem_Write(I2cPin, I2C_ADDR, uint8_t(REGISTER_ADDR), 1, TxBuffer, Len, 1);
}

void ICM45686_HAL_I2C::Read(ICM45686::REGISTER REGISTER_ADDR, uint8_t* RxBuffer, uint8_t Len){

	HAL_I2C_Mem_Read(I2cPin, I2C_ADDR, uint8_t(REGISTER_ADDR), 1, RxBuffer, Len, 1);
}
