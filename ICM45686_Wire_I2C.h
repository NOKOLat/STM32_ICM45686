#ifndef INC_ICM45686_WIRE_I2C_H_
#define INC_ICM45686_WIRE_I2C_H_

#include "ICM45686.h"
#include <Wire.h>

class ICM45686_WIRE_I2C: public ICM45686{

	public:

		ICM45686_WIRE_I2C(uint8_t SDA_pin, uint8_t SCL_pin);

	private:

		void Write(ICM45686::REGISTER reg_addr, uint8_t* TxBuffer, uint8_t Len);
		void Read(ICM45686::REGISTER reg_addr, uint8_t* RxBuffer, uint8_t Len);

		uint8_t SDA_pin = 21;
    uint8_t SCL_pin = 22;
		uint8_t i2c_addr = 0b1101000;

    uint8_t single_byte = 1;
};















#endif