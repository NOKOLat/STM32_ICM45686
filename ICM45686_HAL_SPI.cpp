/* * ICM45686_HAL.cpp
 *
 *  Created on: Dec 17, 2024
 *      Author: aoi25
 */

#include <ICM45686_HAL_SPI.h>

ICM45686_HAL_SPI::ICM45686_HAL_SPI(SPI_HandleTypeDef* SpiPin, GPIO_TypeDef* CsPinType, uint16_t CsPinNum){

	//使用するピンの情報を受け取る
	this->SpiPin = SpiPin;
	this->CsPinType = CsPinType;
	this->CsPinNum = CsPinNum;

}

void ICM45686_HAL_SPI::Write(ICM45686::REGISTER REGISTER_ADDR, uint8_t* TxBuffer, uint8_t Len){

	uint8_t TxTmp[2] = {};

	for(uint8_t i=0; i<Len; i++){

	TxTmp[0] = ((uint8_t)REGISTER_ADDR + i) | 0x00;
	TxTmp[1] = TxBuffer[i];

	HAL_GPIO_WritePin(CsPinType, CsPinNum, GPIO_PIN_RESET);

    HAL_SPI_Transmit(SpiPin, TxTmp, 2, 1000);

    HAL_GPIO_WritePin(CsPinType, CsPinNum, GPIO_PIN_SET);

	}
}

void ICM45686_HAL_SPI::Read(ICM45686::REGISTER REGISTER_ADDR, uint8_t* RxBuffer, uint8_t Len){

	uint8_t RxTmp[2] = {};
	uint8_t TxTmp[2] = {};

	for(uint8_t i=0; i<Len; i++){

		TxTmp[0] = (uint8_t(REGISTER_ADDR) + i) | 0x80;
		TxTmp[1] = 0x00;

		HAL_GPIO_WritePin(CsPinType, CsPinNum, GPIO_PIN_RESET);

		HAL_SPI_TransmitReceive(SpiPin, TxTmp, RxTmp, 2, 100);

		HAL_GPIO_WritePin(CsPinType, CsPinNum, GPIO_PIN_SET);

		RxBuffer[i] = RxTmp[1];
	}
}
