#pragma once
#include "board.h"

// S3-Bot Surabaya v2 - ESP32-S3 DevKitC N16R8
// OLED SSD1306 0.96" 128x64
#define DISPLAY_SDA_PIN 41
#define DISPLAY_SCL_PIN 42
#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 64

// MIC INMP441
#define MIC_I2S_SD_PIN 9
#define MIC_I2S_WS_PIN 45
#define MIC_I2S_SCK_PIN 10

// SPEAKER MAX98357A / NS4168
#define SPEAKER_I2S_DIN_PIN 8
#define SPEAKER_I2S_BCLK_PIN 15
#define SPEAKER_I2S_LRC_PIN 16
// SD MAX98357A -> sambung ke 3.3V (selalu ON)
// GAIN MAX98357A -> sambung ke GND (9dB, aman)

// TOMBOL EKSTERNAL
#define BUTTON_PIN 4
#define BOOT_BUTTON_PIN 0

// RELAY LAMPU - v2 BARU!
#define RELAY_PIN 18
// Relay VCC -> 5V, GND -> GND, IN -> GPIO 18
// Relay aktif LOW: LOW = ON, HIGH = OFF
