// ================================================================
// FILE HEADER: peripherals.h
// Khai báo các hàm quản lý màn hình, cảm biến và nút bấm.
// ================================================================
#ifndef PERIPHERALS_H
#define PERIPHERALS_H

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <Adafruit_AHTX0.h>
#include <Adafruit_TSL2561_U.h>
#include "Adafruit_VEML6075.h"
#include <ArduinoJson.h> // **THÊM THƯ VIỆN ĐỂ TỐI ƯU HÓA**

// Định nghĩa phần cứng
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C
#define BUTTON_PIN 27
#define LUMINOSITY_ADDRESS 0x39

// Khởi tạo các thiết bị ngoại vi
void setupPeripherals();

// Vòng lặp xử lý các sự kiện ngoại vi
void loopPeripherals(bool wifiStatus, bool mqttStatus);

// Hiển thị thông báo chung lên màn hình
void displayMessage(const char* line1, const char* line2, const char* line3);

// Hiển thị trạng thái kết nối
void displayConnectionStatus(bool wifiStatus, bool mqttStatus);

// Lấy dữ liệu cảm biến dưới dạng chuỗi JSON
String getSensorJson();

#endif
