/*
 * ICM45686_HAL_I2C.h
 *
 *  Created on: Mar 15, 2025
 *      Author: Sezakiaoi
 */

#ifndef INC_ICM45686_HAL_I2C_H_
#define INC_ICM45686_HAL_I2C_H_

#include "ICM45686.h"
#include "i2c.h"

class ICM45686_HAL_I2C: public ICM45686{

	public:

		ICM45686_HAL_I2C(I2C_HandleTypeDef* I2cPin);

	private:

		void Write(ICM45686::REGISTER, uint8_t* TxBuffer, uint8_t Len);
		void Read(ICM45686::REGISTER, uint8_t* RxBuffer, uint8_t Len);

		I2C_HandleTypeDef* I2cPin;
		uint8_t I2C_ADDR = 0b1101000 << 1;
};

#endif /* INC_ICM45686_HAL_I2C_H_ */
