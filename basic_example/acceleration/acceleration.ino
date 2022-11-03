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
float acc_offset[3] = {0.f, 0.f, 0.f};
int acc_range_index = 0;

float read_acc(float acc_raw[3], float acc_data[3]){
  uint8_t REG_ACC_OUT = 0x3B;
  uint8_t buf[6];
  Wire.beginTransmission(MPU6500_ADDR);
  Wire.write(REG_ACC_OUT);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU6500_ADDR, 6, true);
  if(Wire.available()){
    for(int i = 0; i < 6; i++){
      buf[i] = Wire.read();
    }
  }
  for(int i = 0; i < 3; i++){
    acc_raw[i] = float(buf[2 * i] << 8 | buf[2 * i + 1]);
    acc_data[i] = acc_raw[i] - (acc_offset[i] / (acc_range_index + 1));
    acc_data[i] *= ((acc_range_index + 1)  /16384.0f);
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

void calc_offset(float acc_offset[3]){
  float acc_raw[3];
  float acc_data[3];
  float acc_accumlator[3] = {0.f, 0.f, 0.f};
  set_accRange(0);
  set_accLPF(6);
  delay(100);
  for(int i = 0;i < 50;i++){
    read_acc(acc_raw, acc_data);
    for(int j = 0;j < 3;j++){
      acc_accumlator[j] += acc_raw[j];
    }
    delay(1);
  }
  for(int i = 0; i < 3;i++){
    acc_offset[i] = acc_accumlator[i] / 50.f;
    acc_offset[2] -= 16384.0f; 
  }
}

void set_accRange(int reg_index){
  uint8_t REG_ACC_CONFIG = 0x1C;
  uint8_t reg[4] = {0x00, 0x08, 0x10, 0x18};
  /*
   * 0x00 => ±2g
   * 0x08 => ±4g
   * 0x10 => ±8g
   * 0x18 => ±16g
   */
  acc_range_index = reg_index;
  Wire.beginTransmission(MPU6500_ADDR);
  Wire.write(REG_ACC_CONFIG);
  Wire.write(reg[reg_index]);
  Wire.endTransmission(true);
  delay(10);
}

void set_accLPF(int reg_index){
  uint8_t ACC_CONFIG = 0x1D;
  uint8_t reg[7] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06};
  /*
   * 0x00 => Bandwidth : 460(Hz) Delay : 1.94(ms)
   * 0x01 => Bandwidth : 184(Hz) Delay : 5.8(ms)
   * 0x02 => Bandwidth : 92(Hz) Delay : 7.8(ms)
   * 0x03 => Bandwidth : 41(Hz) Delay : 11.8(ms)
   * 0x04 => Bandwidth : 20(Hz) Delay : 19.8(ms)
   * 0x05 => Bandwidth : 10(Hz) Delay : 35.7(ms)
   * 0x06 => Bandwidth : 5(Hz) Delay : 66.96(ms)
   */
  Wire.beginTransmission(MPU6500_ADDR);
  Wire.write(ACC_CONFIG);
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
  calc_offset(acc_offset);
  set_accRange(0);
  set_accLPF(6);
  set_smplrtDiv(5);
}
 
void loop(){
  float acc_raw[3];
  float acc_data[3];
  read_acc(acc_raw, acc_data);
  Serial.println("%%%%%%%%%%%%%%%Acceleration data%%%%%%%%%%%%%%%");
  Serial.print("acc_raw_x: ");
  Serial.print(acc_raw[0]);
  Serial.print(", acc_raw_y: ");
  Serial.print(acc_raw[1]);
  Serial.print(", acc_raw_z: ");
  Serial.println(acc_raw[2]);
  Serial.print("acc_x: ");
  Serial.print(acc_data[0]);
  Serial.print("g");
  Serial.print(", acc_y: ");
  Serial.print(acc_data[1]);
  Serial.print("g");
  Serial.print(", acc_z: ");
  Serial.println(acc_data[2]);
  Serial.println("g");
}
