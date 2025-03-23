/*
 * ICM45686.h
 *
 *  Created on: Dec 17, 2024
 *      Author: aoi25
 */

#ifndef INC_ICM45686_H_
#define INC_ICM45686_H_

#include <cstdint>
#include <bitset>

class ICM45686{

	public:

		enum class REGISTER: uint8_t{

			ACCEL_DATA_X1_UI = 0x00,
			PWR_MGMT0        = 0x10,
			ACCEL_CONFIG     = 0x1B,
			GYRO_CONFIG 	 = 0x1C,
			WHO_AM_I   		 = 0x72
		};

		enum class Mode: uint8_t{

			off = 0x00,
			standby,
			low_power,
			low_noize
		};

		enum class AccelScale: uint8_t{

			scale_32g = 0x00,
			scale_16g,
			scale_08g,
			scale_04g,
			scale_02g
		};

		enum class GyroScale: uint8_t{

			scale_4000dps = 0x00,
			scale_2000dps,
			scale_1000dps,
			scale_0500dps,
			scale_0250dps,
			scale_0125dps,
			scale_0062dps,
			scale_0031dps,
			scale_0015dps,
			scale_0006dps

		};

		enum class ODR: uint8_t{

			rate_6400hz = 3,
			rate_3200hz,
			rate_1600hz,
			rate_0800hz,
			rate_0400hz,
			rate_0200hz,
			rate_0100hz,
			rate_0050hz,
			rate_0025hz,
			rate_0012hz,
			rate_0006hz,
			rate_0003hz,
			rate_0001hz
		};

		uint8_t Connection();
		uint8_t AccelConfig(ICM45686::Mode, ICM45686::AccelScale, ICM45686::ODR);
		uint8_t GyroConfig(ICM45686::Mode, ICM45686::GyroScale, ICM45686::ODR);
		uint8_t GetRawData(int16_t AccelBuffer[3], int16_t GyroBuffer[3]);
		uint8_t GetData(float AccelBuffer[3], float GyroBuffer[3]);
		uint8_t Calibration(uint16_t Count);

	private:

		//Buffer
		uint8_t RawData[12] = {};;
		uint8_t PreData = 0;

		//Config
		float AccelScaleValue = 0;
		float GyroScaleValue = 0;
		uint8_t AccelModeTmp = 0;
		uint8_t GyroModeTmp = 0;

		//OffSet
		int32_t AccelOffset[3] = {};
		int32_t GyroOffset[3] = {};

		//Calc
		float G = 9.80665;

		//Read,Write
		virtual void Read(ICM45686::REGISTER, uint8_t* RxBuffer, uint8_t Len){}
		virtual void Write(ICM45686::REGISTER, uint8_t* TxBuffer, uint8_t Len){}
};

#endif /* INC_ICM45686_H_ */
