
# ESP32-S3 DevKitC N16R8 Custom 0.96 OLED - Build Guide

Wiring sesuai request:
- OLED SSD1306 0.96" SDA=41 SCL=42 ADDR=0x3C
- INMP441 SD=9 WS=45 SCK=10 L/R=GND
- MAX98357A DIN=8 BCLK=15 LRC=16

## Cara Build BIN Custom:

1. Install ESP-IDF 5.4 / 6.1
2. Clone xiaozhi:
   git clone https://github.com/78/xiaozhi-esp32
   cd xiaozhi-esp32

3. Copy board custom:
   - Copy folder main/boards/esp32-s3-devkitc-1 menjadi main/boards/esp32-s3-devkitc-n16r8-096oled
   - Ganti isi board.h dengan file board_n16r8_custom.h di zip ini

4. Build:
   idf.py set-target esp32s3
   idf.py menuconfig -> pilih Board = esp32-s3-devkitc-n16r8-096oled
   idf.py build
   -> hasil di build/xiaozhi.bin

5. Flash:
   idf.py -p COMx flash monitor
   atau pakai ESP32 Flash Tool dengan bin hasil build

Alternatif cepat: pakai GitHub Actions di repo xiaozhi-esp32, pilih board custom.

BIN final akan bernama: v2.2.4_n16r8-custom-0.96oled-wifi.bin
