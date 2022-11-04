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

float read_temp(){
  uint8_t TEMP_OUT = 0x41;
  uint8_t buf[2];
  float temp_data;
  Wire.beginTransmission(MPU6500_ADDR);
  Wire.write(TEMP_OUT);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU6500_ADDR, 2, true);
  if(Wire.available()){
    for(int i = 0; i < 2; i++){
      buf[i] = Wire.read();
    }
  }
  temp_data = float(buf[0] << 8 | buf[1]);
  temp_data = temp_data / 333.87f + 21.0;
  return temp_data;
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

void set_tempLPF(int reg_index){
  uint8_t REG_CONFIG = 0x1A;
  uint8_t reg[7] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06};
  /*
   * 0x00 => Bandwidth : 4000(Hz) Delay : 0.04(ms)
   * 0x01 => Bandwidth : 188(Hz) Delay : 1.9(ms)
   * 0x02 => Bandwidth : 98(Hz) Delay : 2.8(ms)
   * 0x03 => Bandwidth : 42(Hz) Delay : 4.8(ms)
   * 0x04 => Bandwidth : 20(Hz) Delay : 8.3(ms)
   * 0x05 => Bandwidth : 10(Hz) Delay : 13.4(ms)
   * 0x06 => Bandwidth : 5(Hz) Delay : 18.6(ms)
   */
  Wire.beginTransmission(MPU6500_ADDR);
  Wire.write(REG_CONFIG);
  Wire.write(reg[reg_index]);
  Wire.endTransmission(true);
  delay(10);
}

void setup(){
  Serial.begin(115200);
  Wire.begin();
  reset_imu();
  set_tempLPF(6);
}
 
void loop(){
  float temp_data;
  temp_data = read_temp();
  Serial.print("Temperature: ");
  Serial.print(temp_data);
  Serial.println("°C");
  delay(100);
}
