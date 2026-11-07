#include "esp32_s3_devkitc_n16r8_096oled.h"
#include "system_reset.h"
#include "audio_codec.h"
#include "display.h"
#include "application.h"
#include "button.h"
#include "config.h"
#include "i2c_device.h"
#include "iot/thing_manager.h"
#include "esp_log.h"
#include <wifi_station.h>
#include <esp_wifi.h>

#define TAG "S3Bot_N16R8"

// S3-Bot Surabaya - ESP32-S3-WROOM-1 N16R8
// Wiring:
// OLED SDA 41, SCL 42
// MIC SD 9, WS 45, SCK 10
// SPK DIN 8, BCLK 15, LRC 16
// BTN GPIO 4 -> GND

class Esp32S3DevkitcN16r8 : public Board {
private:
    i2c_master_bus_handle_t i2c_bus_;
    Button boot_button_;
    Display* display_;
    SpiramAwareAudioCodec audio_codec_;

    void InitializeI2c() {
        i2c_master_bus_config_t bus_config = {
            .i2c_port = I2C_NUM_0,
            .sda_io_num = (gpio_num_t)DISPLAY_SDA_PIN,
            .scl_io_num = (gpio_num_t)DISPLAY_SCL_PIN,
            .clk_source = I2C_CLK_SRC_DEFAULT,
            .glitch_ignore_cnt = 7,
            .intr_priority = 0,
            .trans_queue_depth = 0,
            .flags = {.enable_internal_pullup = true},
        };
        ESP_ERROR_CHECK(i2c_new_master_bus(&bus_config, &i2c_bus_));
    }

    void InitializeDisplay() {
        auto display = new Ssd1306Display(i2c_bus_, DISPLAY_WIDTH, DISPLAY_HEIGHT, 0x3C, false);
        display_ = display;
    }

    void InitializeButtons() {
        boot_button_.OnClick([this]() {
            auto& app = Application::GetInstance();
            if (app.GetDeviceState() == kDeviceStateStarting && !WifiStation::GetInstance().IsConnected()) {
                ResetWifiConfiguration();
            }
            app.ToggleChatState();
        });
        // Tombol eksternal GPIO 4
        auto btn4 = new Button(BUTTON_PIN);
        btn4->OnClick([this]() {
            auto& app = Application::GetInstance();
            app.ToggleChatState();
        });
    }

    void InitializeAudioCodec() {
        audio_codec_.Initialize(
            MIC_I2S_SD_PIN, MIC_I2S_WS_PIN, MIC_I2S_SCK_PIN,
            SPEAKER_I2S_DIN_PIN, SPEAKER_I2S_BCLK_PIN, SPEAKER_I2S_LRC_PIN
        );
    }

public:
    Esp32S3DevkitcN16r8() : boot_button_(BOOT_BUTTON_PIN), audio_codec_() {
        InitializeI2c();
        InitializeDisplay();
        InitializeButtons();
        InitializeAudioCodec();
    }

    virtual Display* GetDisplay() override { return display_; }
    virtual AudioCodec* GetAudioCodec() override { return &audio_codec_; }
};

DECLARE_BOARD(Esp32S3DevkitcN16r8);
