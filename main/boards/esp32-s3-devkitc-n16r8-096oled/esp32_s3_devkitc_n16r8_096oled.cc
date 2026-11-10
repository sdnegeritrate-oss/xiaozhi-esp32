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
#include <math.h>

#define TAG "S3Bot_N16R8_v2"

// ============================================================
// S3-Bot Surabaya v2 - ESP32-S3-WROOM-1 N16R8
// Wiring:
// OLED SDA 41, SCL 42
// MIC SD 9, WS 45, SCK 10
// SPK DIN 8, BCLK 15, LRC 16, SD->3.3V, GAIN->GND
// BTN GPIO 4 -> GND
// RELAY GPIO 18 -> Relay IN (VCC 5V, GND GND)
// ============================================================

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

    // ===== v2 BARU: MATA MELIRIK + RELAY =====
    void InitializeRelay() {
        // Relay setup
        gpio_set_direction((gpio_num_t)RELAY_PIN, GPIO_MODE_OUTPUT);
        gpio_set_level((gpio_num_t)RELAY_PIN, 1); // OFF awal (aktif LOW)
        ESP_LOGI(TAG, "Relay GPIO %d siap - LOW=ON, HIGH=OFF", RELAY_PIN);

        // Daftarkan ke Xiaozhi IoT / MCP
        // Perintah suara: "Nyalakan lampu!" / "Matikan lampu!"
        auto& thing_manager = ThingManager::GetInstance();
        thing_manager.AddThing("Relay", "Lampu S3-Bot", [this](const std::string& action) {
            if (action == "on" || action == "turn_on") {
                gpio_set_level((gpio_num_t)RELAY_PIN, 0);
                ESP_LOGI(TAG, "Lampu ON - Relay GPIO 18 LOW");
            } else if (action == "off" || action == "turn_off") {
                gpio_set_level((gpio_num_t)RELAY_PIN, 1);
                ESP_LOGI(TAG, "Lampu OFF - Relay GPIO 18 HIGH");
            }
        });
    }

    // Mata melirik pakai sin() - dipanggil tiap frame di loop display
    // look_x: -10 sampai +10 (kiri-kanan), look_y: -3 sampai +3 (atas-bawah)
    // Cara pakai: di Application loop, hitung millis() lalu gambar mata di posisi ini
    void DrawGlancingEyes(int time_ms) {
        int look_x = (int)(sin(time_ms / 2000.0 * 3.14159) * 10);
        int look_y = (int)(cos(time_ms / 3000.0 * 3.14159) * 3);
        // Kedip tiap 4 detik: tutup mata 150ms
        bool blink = (time_ms % 4000) < 150;
        if (blink) {
            // Mata tertutup: gambar garis
            // display_->DrawLine(28, 32, 52, 32);
            // display_->DrawLine(76, 32, 100, 32);
        } else {
            // Mata kiri + kanan ikut look_x / look_y
            // Lingkaran mata: (40+look_x, 32+look_y) r=12, (88+look_x, 32+look_y) r=12
            // Pupil: r=5 di tengah mata
            // Implementasi display tergantung driver SSD1306 di Xiaozhi
        }
        (void)look_x; (void)look_y;
    }

public:
    Esp32S3DevkitcN16r8() : boot_button_(BOOT_BUTTON_PIN), audio_codec_() {
        InitializeI2c();
        InitializeDisplay();
        InitializeButtons();
        InitializeAudioCodec();
        InitializeRelay(); // v2: Relay + IoT
    }

    virtual Display* GetDisplay() override { return display_; }
    virtual AudioCodec* GetAudioCodec() override { return &audio_codec_; }
};

DECLARE_BOARD(Esp32S3DevkitcN16r8);
