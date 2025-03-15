# STM32 ICM45686

STM32のHALライブラリを用いてICM45686の6軸センサーデータを取得するためのコードです

I2C通信とSPI通信の両方に対応しています

## I2C通信

- FastPulsMode(~1Mbps)まで対応しています

ピンの設定と通信速度の設定のみ行ってください

### プロジェクトフォルダに追加するファイル
```
ICM45686.h / ICM45686.cpp
ICM45686_HAL_I2C.h / ICM45686_HAL_I2C.cpp
```

### ピンの設定

一般的なI2C通信です

| 設定するピン | ICM45686のピン |
| ---- | ---- | 
| I2Cx_SCK | SCL |
| I2Cx_SDA | SDA|

### サンプルコード

```cpp
#include "ICM45686_HAL_I2C.h"

//使用するI2Cの番号を入力
ICM45686_HAL_I2C icm(&hi2c1);

//Buffer
int16_t Gyro_Data[3] = {};
int16_t Accel_Data[3] = {};

int main(void){

	icm.Connection();
	icm.Accel_Config(icm.Mode::low_noize, icm.Accel_Scale::scale_02g, icm.ODR::rate_6400hz);
	icm.Gyro_Config(icm.Mode::low_noize, icm.Gyro_Scale::scale_0250dps, icm.ODR::rate_6400hz);

  while(1){

  	icm.Get_Data(Accel_Data, Gyro_Data);
  }
}
```

## SPI通信

- 4本線通信
- 24Mbpsまで対応しています

### プロジェクトフォルダに追加するファイル
```
ICM45686.h / ICM45686.cpp
ICM45686_HAL_SPI.h / ICM45686_HAL_SPI.cpp
```

### ピンの設定

GPIO_OutputでSPIのデバイス選択を行っています

| 設定するピン | ICM45686のピン |
| ---- | ---- | 
| SPIx_SCK | SCLK |
| SPIx_MISO | SDIO / SDI|
| SPIx_MOSI | SDO |
| GPIO_Output | CS|

### SPIx
![スクリーンショット (523)](https://github.com/user-attachments/assets/dfb9b2da-9f23-441c-ba0c-fa767348adbd)


### GPIO
![スクリーンショット (524)](https://github.com/user-attachments/assets/e1aaffec-eda2-4d0e-bdeb-c1da288f8b3b)



### サンプルコード

```cpp
#include "ICM45686_HAL_SPI.h"

//使用するSPIとGPIOのピンを入力
ICM45686_HAL_SPI icm(&hspi1, GPIOB, GPIO_PIN_6);

//Buffer
int16_t Gyro_Data[3] = {};
int16_t Accel_Data[3] = {};

int main(void){

	icm.Connection();
	icm.Accel_Config(icm.Mode::low_noize, icm.Accel_Scale::scale_02g, icm.ODR::rate_6400hz);
	icm.Gyro_Config(icm.Mode::low_noize, icm.Gyro_Scale::scale_0250dps, icm.ODR::rate_6400hz);

  while(1){

  	icm.Get_Data(Accel_Data, Gyro_Data);
  }
}
```
## その他

- サンプルコードは、起動時に実行される部分とメインループで実行する部分にわけて書いてあります

    使う環境に合わせてください

- Config関数の中身はenum classで定義してあります

    設定を変えたい際は、```icm.Mode::```などと入力すると候補が出てきます

![スクリーンショット (493)](https://github.com/user-attachments/assets/1dff5040-14f2-4d4b-9303-17537af35aec)

- ConnectionとConfig関数には戻り値が設定されています

  特にConnection関数で通信確立を確認する場合はエラーハンドリングを行うようにしてください

  (return 1を判定できるように書いてください）

