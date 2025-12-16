// ================================================================
// FILE HEADER: wifi_handler.h
// Khai báo các hàm quản lý kết nối Wi-Fi.
// ================================================================

#ifndef WIFI_HANDLER_H
#define WIFI_HANDLER_H

#include <Arduino.h>
#include <WiFi.h>
#include <FS.h>
#include <LittleFS.h>
#include <WiFiManager.h>
#include <ArduinoJson.h>
#include "peripherals.h" // Để hiển thị thông báo lên OLED

// Định nghĩa tên file và số lượng mạng tối đa
#define CONFIG_FILE "/wifi_multi_config.json"
#define MAX_NETWORKS 5

// Hàm chính để thiết lập Wi-Fi.
// Sẽ thử kết nối mạng đã lưu trước, nếu không được sẽ mở portal.
// Trả về true nếu kết nối thành công, false nếu thất bại.
bool setupWiFi();

#endif