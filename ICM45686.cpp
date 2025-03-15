/*
 * ICM45686.cpp
 *
 *  Created on: Dec 17, 2024
 *      Author: aoi25
 */

#include <ICM45686.h>
#include <bitset>

/**
 * @brief センサーとの接続を確認
 *
 * ICM45686のWHO_AM_Iレジスタとの通信を用いて、接続を確認します
 * 最大100回の再試行をします
 *
 * @return uint8_t 0: 接続成功、1: 接続失敗
 */
uint8_t ICM45686::Connection(){
    uint8_t Product_ID = 0x00;
    uint8_t Error = 0;

    // 0xE9なら成功
    while(Product_ID != 0xE9){

    	Read(ICM45686::REGISTER::WHO_AM_I, &Product_ID, 1);
        Error++;

        if(Error >= 100){

            return 1; // 接続失敗
        }
    }

    return 0; // 接続成功
}

/* @brief 加速度センターの設定
 *
 * PWR_MGMT0レジスタで電力モードの設定
 * ACCEL_CONFIGレジスタでScaleとODRの設定を行います
 * 最大で100回の再試行をします
 *
 * @param Mode 電力モードの設定
 * @param Scale 加速度センサーのスケール設定
 * @param ODR 出力データレートの設定
 *
 * @return uint8_t 0: 接続成功、1: PWR_MGMT0接続失敗、2: ACCEL_CONFIG接続失敗
 */
uint8_t ICM45686::Accel_Config(ICM45686::Mode Mode, ICM45686::Accel_Scale Scale, ICM45686::ODR ODR){

	//現在の値を取得
	uint8_t Now_Mode = 0x00;
	Read(ICM45686::REGISTER::PWR_MGMT0, &Now_Mode, 1);

	//取得した設定から書き込みデータを作成
	uint8_t  Command = Now_Mode | (uint8_t)Mode;

	//値が一致するまで書き込みを行う
	uint8_t Error = 0;
	while(Now_Mode != Command){

		Write(ICM45686::REGISTER::PWR_MGMT0, &Command, 1);

		Read(ICM45686::REGISTER::PWR_MGMT0, &Now_Mode, 1);

		Error ++;

		if(Error > 100){

			return 1;//PWR_MGMT0接続失敗
		}
	}


	//ODRとscaleを書き込み
	Command = (uint8_t)ODR + (((uint8_t)Scale) << 4);

	//値が一致するまで書き込みを行う
	Now_Mode = 0;
	Error = 0;
	while(Now_Mode != Command){

		Write(ICM45686::REGISTER::ACCEL_CONFIG, &Command, 1);

		Read(ICM45686::REGISTER::ACCEL_CONFIG, &Now_Mode, 1);

		Error ++;

		if(Error > 100){

			return 2;//ACCEL_CONFIG接続失敗
		}
	}

	return 0;
}

/* @brief ジャイロセンターの設定
 *
 * PWR_MGMT0レジスタで電力モードの設定
 * Gyro_CONFIGレジスタでScaleとODRの設定を行います
 * 最大で100回の再試行をします
 *
 * @param Mode 電力モードの設定
 * @param Scale ジャイロセンサーのスケール設定
 * @param ODR 出力データレートの設定
 *
 * @return uint8_t 0: 接続成功、1: PWR_MGMT0接続失敗、2: ACCEL_CONFIG接続失敗
 */
uint8_t ICM45686::Gyro_Config(ICM45686::Mode Mode, ICM45686::Gyro_Scale Scale,  ICM45686::ODR ODR){

	//現在の値を取得
	uint8_t Now_Mode = 0x00;
	Read(ICM45686::REGISTER::PWR_MGMT0, &Now_Mode, 1);

	//取得した設定から書き込みデータを作成
	uint8_t Command = Now_Mode | (uint8_t)Mode << 2;

	//値が一致するまで書き込みを行う
	uint8_t Error = 0;
	while(Command != Now_Mode){

		Write(ICM45686::REGISTER::PWR_MGMT0, &Command, 1);

		Read(ICM45686::REGISTER::PWR_MGMT0, &Now_Mode, 1);

		Error ++;

		if(Error > 100){

			return 1;//ACCEL_CONFIG接続失敗
		}
	}

	//ODRとscaleを書き込み
	Command = (uint8_t)ODR + ((uint8_t)Scale << 4);

	//値が一致するまで書き込みを行う
	Now_Mode = 0;
	Error = 0;
	while(Now_Mode != Command){

		Write(ICM45686::REGISTER::GYRO_CONFIG, &Command, 1);

		Read(ICM45686::REGISTER::GYRO_CONFIG, &Now_Mode, 1);

		Error ++;

		if(Error > 100){

			return 2;//ACCEL_CONFIG接続失敗
		}
	}



	return 0;
}

/* @brief 加速度センサーとジャイロセンサーからのデータを取得
 *
 * @param int16_t Accel_Data[3] 加速度データを入れる配列
 * @param int16_t Gyro_Data[3]  角速度データを入れる配列
 *
 * @return uint8_t 常に0を返します。取得失敗時は前回と同じ値を返します。
 */
uint8_t ICM45686::Get_Data(int16_t Accel_Data[3], int16_t Gyro_Data[3]){

	Read(ICM45686::REGISTER::ACCEL_DATA_X1_UI, Raw_Data, 12);

	Accel_Data[0]  = (int16_t)(Raw_Data[0] | Raw_Data[1] << 8);
	Accel_Data[1]  = (int16_t)(Raw_Data[2] | Raw_Data[3] << 8);
	Accel_Data[2]  = (int16_t)(Raw_Data[4] | Raw_Data[5] << 8);

	Gyro_Data[0]  = (int16_t)(Raw_Data[6]  | Raw_Data[7] << 8);
	Gyro_Data[1]  = (int16_t)(Raw_Data[8]  | Raw_Data[9] << 8);
	Gyro_Data[2]  = (int16_t)(Raw_Data[10] | Raw_Data[11] << 8);

	return 0;
}
