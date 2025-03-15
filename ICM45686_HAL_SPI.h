/*
 * ICM45686_HAL_SPI.h
 *
 *  Created on: Dec 17, 2024
 *      Author: aoi25
 */

#ifndef INC_ICM45686_HAL_H_
#define INC_ICM45686_HAL_H_

#include "ICM45686.h"
#include "spi.h"
#include "gpio.h"

class ICM45686_HAL_SPI: public ICM45686{

	public:

		ICM45686_HAL_SPI(SPI_HandleTypeDef* SpiPin, GPIO_TypeDef* CsPinType, uint16_t CsPinNum);

	private:

		void Write(ICM45686::REGISTER, uint8_t* TxBuffer, uint8_t Len);
		void Read(ICM45686::REGISTER, uint8_t* RxBuffer, uint8_t Len);

		SPI_HandleTypeDef* SpiPin;
		GPIO_TypeDef* CsPinType;
		uint16_t CsPinNum;
};

#endif /* INC_ICM45686_HAL_H_ */
