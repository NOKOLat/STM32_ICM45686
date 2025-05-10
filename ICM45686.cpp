/*
 * ICM45686.cpp
 *
 *  Created on: Dec 17, 2024
 *      Author: aoi25
 */

#include <ICM45686.h>
#include <bitset>
#include <cmath>

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
uint8_t ICM45686::AccelConfig(ICM45686::Mode Mode, ICM45686::AccelScale Scale, ICM45686::ODR ODR){

	//現在の値を取得できるまで待機
	uint8_t NowMode = 0x00;

	uint8_t Error = 0;
	while(NowMode != GyroModeTmp){

		Read(ICM45686::REGISTER::PWR_MGMT0, &NowMode, 1);

		Error ++;

		if(Error > 100){

			return 1;//PWR_MGMT0接続失敗
		}
	}

	//取得した設定から書き込みデータを作成
	uint8_t  Command = NowMode | (uint8_t)Mode;

	//値が一致するまで書き込みを行う
	Error = 0;
	while(NowMode != Command){

		Write(ICM45686::REGISTER::PWR_MGMT0, &Command, 1);

		Read(ICM45686::REGISTER::PWR_MGMT0, &NowMode, 1);

		Error ++;

		if(Error > 100){

			return 1;//PWR_MGMT0接続失敗
		}
	}

	//書き込んだModeの保存
	AccelModeTmp = (uint8_t)Mode;

	//ODRとscaleを書き込み
	Command = (uint8_t)ODR + (((uint8_t)Scale) << 4);

	//値が一致するまで書き込みを行う
	NowMode = 0;
	Error = 0;
	while(NowMode != Command){

		Write(ICM45686::REGISTER::ACCEL_CONFIG, &Command, 1);

		Read(ICM45686::REGISTER::ACCEL_CONFIG, &NowMode, 1);

		Error ++;

		if(Error > 100){

			return 2;//ACCEL_CONFIG接続失敗
		}
	}

	//Scaleの値を保存
	AccelScaleValue = 32.0 / pow(2,(uint8_t)Scale);

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
uint8_t ICM45686::GyroConfig(ICM45686::Mode Mode, ICM45686::GyroScale Scale,  ICM45686::ODR ODR){

	//現在の値を取得
	uint8_t NowMode = 0x00;

	//値が取得できるまで待機
	uint8_t Error = 0;
	while(NowMode != AccelModeTmp){

		Read(ICM45686::REGISTER::PWR_MGMT0, &NowMode, 1);

		Error ++;

		if(Error > 100){

			return 1;
		}
	}

	//取得した設定から書き込みデータを作成
	uint8_t Command = NowMode | (uint8_t)Mode << 2;

	//値が一致するまで書き込みを行う
	Error = 0;
	while(Command != NowMode){

		Write(ICM45686::REGISTER::PWR_MGMT0, &Command, 1);

		Read(ICM45686::REGISTER::PWR_MGMT0, &NowMode, 1);

		Error ++;

		if(Error > 100){

			return 1;
		}
	}

	//書き込んだModeの保存
	GyroModeTmp = (uint8_t)Mode;

	//ODRとscaleを書き込み
	Command = (uint8_t)ODR + ((uint8_t)Scale << 4);

	//値が一致するまで書き込みを行う
	NowMode = 0;
	Error = 0;
	while(NowMode != Command){

		Write(ICM45686::REGISTER::GYRO_CONFIG, &Command, 1);

		Read(ICM45686::REGISTER::GYRO_CONFIG, &NowMode, 1);

		Error ++;

		if(Error > 100){

			return 2;
		}
	}

	//Scaleの値を保存
	GyroScaleValue = 4000 / pow(2, (uint8_t)Scale);

	return 0;
}

/* @brief 加速度センサーとジャイロセンサーからの生データを取得
 *
 * @param int16_t Accel_Data[3] 加速度データを入れる配列
 * @param int16_t Gyro_Data[3]  角速度データを入れる配列
 *
 * 値が0か前回と同じ場合は、100回まで再試行をします
 * データの判定にはx軸の加速度の下位バイトを使用しています
 *
 * @return uint8_t 成功: 0, 失敗: 1
 */
uint8_t ICM45686::GetRawData(int16_t AccelData[3], int16_t GyroData[3]){

	Read(ICM45686::REGISTER::ACCEL_DATA_X1_UI, RawData, 12);

	//データの受信判定と再試行
	uint8_t Error = 0;
	while(RawData[0] == 0){

		Read(ICM45686::REGISTER::ACCEL_DATA_X1_UI, RawData, 12);
		Error ++;

		if(Error > 10000){
			return 1;
		}
	}

	//取得データの処理
	AccelData[0]  = (int16_t)(RawData[0] | RawData[1] << 8) - AccelOffset[0];
	AccelData[1]  = (int16_t)(RawData[2] | RawData[3] << 8) - AccelOffset[1];
	AccelData[2]  = (int16_t)(RawData[4] | RawData[5] << 8) - AccelOffset[2];

	GyroData[0]  = (int16_t)(RawData[6]  | RawData[7] << 8) - GyroOffset[0];
	GyroData[1]  = (int16_t)(RawData[8]  | RawData[9] << 8) - GyroOffset[1];
	GyroData[2]  = (int16_t)(RawData[10] | RawData[11] << 8) - GyroOffset[2];

	//Bufferをクリア（データ受信判定用）
	RawData[0] = 0;

	return 0;
}

uint8_t ICM45686::GetData(float AccelData[3], float GyroData[3]){

	int16_t AccelBuffer[3] = {};
	int16_t GyroBuffer[3] = {};

	//データの取得
	if(GetRawData(AccelBuffer, GyroBuffer) == 1){

		//取得失敗
		return 1;
	}

	//単位を変換
	for(uint8_t i=0; i<3; i++){

		AccelData[i] = AccelBuffer[i] * G * AccelScaleValue / 32768.0;
		GyroData[i]  = GyroBuffer[i] * GyroScaleValue / 32768.0;
	}
	return 0;
}

uint8_t ICM45686::Calibration(uint16_t Count){

	int16_t AccelData[3] = {};
	int16_t GyroData[3] = {};

	int32_t AccelTmp[3] = {};
	int32_t GyroTmp[3] = {};

	//値を取得して合計を保存
	for(uint16_t i=0; i < Count; i++){

		if(GetRawData(AccelData, GyroData) == 1){

			return 1;
		}

		for(uint8_t j=0; j<3; j++){

			AccelTmp[j] += AccelData[j];
			GyroTmp[j] += GyroData[j];
		}

		for(uint32_t k=0; k < 10000; k++);
	}

	//平均値を計算
	for(uint8_t k=0; k<3; k++){

		AccelOffset[k] = AccelTmp[k] / Count;
		GyroOffset[k] = GyroTmp[k] / Count;
	}

	//重力加速度をオフセットに含める
	AccelOffset[2] -= 32768 / AccelScaleValue;

	return 0;
}
