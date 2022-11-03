# zipino rev1.0 [![](https://img.shields.io/badge/Arduino%20Uno-Compatible-blue?style=flat-square&logo=arduino)](https://github.com/Alpaca-zip/zipino-project/rev1.0) [![](https://img.shields.io/badge/Project%20Status-Prototyping-yellow?style=flat-square)](https://github.com/Alpaca-zip/zipino-project/rev1.0)  
This is an Arduino compatible board equipped with the MPU6500 IMU sensor.
## Bill of Materials:wrench:
- ATMEGA328P-AU
- FT232RL-REEL
- NCP1117ST50T3G
- LM358D
- FDN340P
- 1kΩ Chip Resistor (×4)
- 10kΩ Chip Resistor (×5)
- 47uF Aluminum Electrolytic Capacitors (×2)
- 100nF Multilayer Ceramic Capacitors (×9)
- 1uF Multilayer Ceramic Capacitors
- 10nF Multilayer Ceramic Capacitors
- White light Light Emitting Diodes
- Emerald light Light Emitting Diodes
- MF-MSMF050-2
- S2BA-13-F
- Surface Mount 5 Female Mini-B SMD USB Connectors
- DC-470-2.1GP
- CSTNE16M0V530000R0
- White Rectangle button Brick nogging SPST SMD Tactile Switches
- 1x2P Straight,P=2.54mm Pin Headers (×2)
- 1x8P Straight,P=2.54mm Female Headers (×2)
- 1x6P Straight,P=2.54mm Female Headers
- 1x10P Straight,P=2.54mm Female Headers
## How to Burn the Bootloader:fire:
The easiest way to burn the bootloader to zipino is using a second Arduino board as a programmer.
### 1. Connect the board
Connect the PROGRAMMER BOARD and ZIPINO BOARD as shown in the table below.
|  PROGRAMMER BOARD  |  ZIPINO BOARD  |
| :----------------: | :------------: |
| 5V | 5V |
| GND | GND |
| D10 | RST |
| D11 | D11 |
| D12 | D12 |
| D13 | D13 |
### 2. Burn the bootloader from Arduino IDE.
  1. Open Arduino IDE.
