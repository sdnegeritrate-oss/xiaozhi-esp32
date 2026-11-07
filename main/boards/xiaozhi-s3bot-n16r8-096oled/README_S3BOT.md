# S3-Bot - Xiaozhi Custom Board N16R8

Board: ESP32-S3 DevKitC N16R8 (16MB Flash + 8MB PSRAM)
OLED: SSD1306 0.96" 128x64 - SDA 41, SCL 42
Mic: INMP441 - SD 9, WS 45, SCK 10
Speaker: MAX98357A / NS4168 - DIN 8, BCLK 15, LRC 16
Button: GPIO 4 -> GND

CARA UPLOAD KE GITHUB:
1. Fork https://github.com/78/xiaozhi-esp32
2. Upload folder main/boards/esp32-s3-devkitc-n16r8-096oled/ ini ke fork kamu
3. Buka tab Actions -> Run workflow
   Board: esp32-s3-devkitc-n16r8-096oled
   Language: Indonesian
   Wake word: No wake word
4. Tunggu Completed, download Artifacts -> firmware.zip
5. Flash via flash.xiaozhi.me

WIRING HARUS SAMA PERSIS DENGAN board.h!
