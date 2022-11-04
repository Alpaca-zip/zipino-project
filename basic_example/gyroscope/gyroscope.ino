/**
 * Copyright (C) 2022  Alpaca-zip
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <Wire.h>
#define MPU6500_ADDR 0x68
float gyro_offset[3] = {0.f, 0.f, 0.f};
int gyro_range_index = 0;

float read_gyro(float gyro_raw[3], float gyro_data[3]){
  uint8_t REG_GYRO_OUT = 0x43;
  uint8_t buf[6];
  Wire.beginTransmission(MPU6500_ADDR);
  Wire.write(REG_GYRO_OUT);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU6500_ADDR, 6, true);
  if(Wire.available()){
    for(int i = 0; i < 6; i++){
      buf[i] = Wire.read();
    }
  }
  for(int i = 0; i < 3; i++){
    gyro_raw[i] = float(buf[2 * i] << 8 | buf[2 * i + 1]);
    gyro_data[i] = gyro_raw[i] - (gyro_offset[i] / (gyro_range_index + 1));
    gyro_data[i] *= ((gyro_range_index + 1) * 250.f /32768.0f);
  }
}

void reset_imu(){
  uint8_t REG_PWR_MGMT_1 = 0x6B;
  uint8_t RESET_VALUE = 0x80;
  Wire.beginTransmission(MPU6500_ADDR);
  Wire.write(REG_PWR_MGMT_1);
  Wire.write(RESET_VALUE);
  Wire.endTransmission(true);
  delay(10);
}

void calc_offset(float gyro_offset[3]){
  float gyro_raw[3];
  float gyro_data[3];
  float gyro_accumlator[3] = {0.f, 0.f, 0.f};
  set_gyroRange(0);
  set_gyroLPF(6);
  delay(100);
  for(int i = 0;i < 50;i++){
    read_gyro(gyro_raw, gyro_data);
    for(int j = 0;j < 3;j++){
      gyro_accumlator[j] += gyro_raw[j];
    }
    delay(1);
  }
  for(int i = 0; i < 3;i++){
    gyro_offset[i] = gyro_accumlator[i] / 50.f;
  }
}

void set_gyroRange(int reg_index){
  uint8_t REG_GYRO_CONFIG = 0x1B;
  uint8_t reg[4] = {0x00, 0x08, 0x10, 0x18};
  /*
   * 0x00 => ±250dps
   * 0x08 => ±500dps
   * 0x10 => ±1000dps
   * 0x18 => ±2000dps
   */
  gyro_range_index = reg_index;
  Wire.beginTransmission(MPU6500_ADDR);
  Wire.write(REG_GYRO_CONFIG);
  Wire.write(reg[reg_index]);
  Wire.endTransmission(true);
  delay(10);
}

void set_gyroLPF(int reg_index){
  uint8_t REG_CONFIG = 0x1A;
  uint8_t reg[7] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06};
  /*
   * 0x00 => Bandwidth : 250(Hz) Delay : 0.97(ms)
   * 0x01 => Bandwidth : 184(Hz) Delay : 2.9(ms)
   * 0x02 => Bandwidth : 92(Hz) Delay : 3.9(ms)
   * 0x03 => Bandwidth : 41(Hz) Delay : 5.9(ms)
   * 0x04 => Bandwidth : 20(Hz) Delay : 9.9(ms)
   * 0x05 => Bandwidth : 10(Hz) Delay : 17.85(ms)
   * 0x06 => Bandwidth : 5(Hz) Delay : 33.48(ms)
   */
  Wire.beginTransmission(MPU6500_ADDR);
  Wire.write(REG_CONFIG);
  Wire.write(reg[reg_index]);
  Wire.endTransmission(true);
  delay(10);
}

void set_smplrtDiv(uint8_t samplrt_div){
  uint8_t REG_SAMPLRT_DIV = 0x19;
  /*
   * SAMPLE_RATE = INTERNAL_SAMPLE_RATE / (1 + samplrt_div)
   * where INTERNAL_SAMPLE_RATE = 1(kHz)
   * SAMPLRT_DIV is a number 0...255
   */
  Wire.beginTransmission(MPU6500_ADDR);
  Wire.write(REG_SAMPLRT_DIV);
  Wire.write(samplrt_div);
  Wire.endTransmission(true);
  delay(10);
}

void setup(){
  Serial.begin(115200);
  Wire.begin();
  reset_imu();
  delay(1000);
  calc_offset(gyro_offset);
  set_gyroRange(0);
  set_gyroLPF(6);
  set_smplrtDiv(5);
}
 
void loop(){
  float gyro_raw[3];
  float gyro_data[3];
  read_gyro(gyro_raw, gyro_data);
  Serial.println("%%%%%%%%%%%%%%%Gyroscope data%%%%%%%%%%%%%%%");
  Serial.print("gyro_raw_x: ");
  Serial.print(gyro_raw[0]);
  Serial.print(", gyro_raw_y: ");
  Serial.print(gyro_raw[1]);
  Serial.print(", gyro_raw_z: ");
  Serial.println(gyro_raw[2]);
  Serial.print("gyro_x: ");
  Serial.print(gyro_data[0]);
  Serial.print("deg/s");
  Serial.print(", gyro_y: ");
  Serial.print(gyro_data[1]);
  Serial.print("deg/s");
  Serial.print(", gyro_z: ");
  Serial.println(gyro_data[2]);
  Serial.println("deg/s");
}
