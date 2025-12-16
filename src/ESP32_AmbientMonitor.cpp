// ================================================================
// FILE CHÍNH: ESP32_Sensor_Hub.ino
// Chứa setup() và loop(), điều phối hoạt động của các module khác.
// ================================================================

#include "peripherals.h"
#include "wifi_handler.h"
#include "mqtt_handler.h"

// Các biến trạng thái toàn cục
bool wifiConnected = false;
bool mqttConnected = false;

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("\n[KHỞI ĐỘNG DỰ ÁN CẢM BIẾN TRUNG TÂM]");

  // 1. Khởi tạo các thiết bị ngoại vi (OLED, Cảm biến, Nút bấm)
  setupPeripherals();
  displayMessage("Dang khoi dong...", "", "Vui long cho...");

  // 2. Thiết lập và kết nối WiFi
  // Hàm này sẽ thử kết nối mạng đã lưu, hoặc mở portal nếu cần.
  wifiConnected = setupWiFi();

  // 3. Nếu có WiFi, kết nối MQTT
  if (wifiConnected) {
    mqttConnected = connectToMQTT();
  }

  // 4. Hiển thị trạng thái kết nối ban đầu
  displayConnectionStatus(wifiConnected, mqttConnected);
  delay(3000);
}

void loop() {
  // Kiểm tra kết nối WiFi định kỳ
  if (wifiConnected && WiFi.status() != WL_CONNECTED) {
    Serial.println("Mất kết nối WiFi, thử kết nối lại...");
    displayMessage("Mat ket noi WiFi", "Dang thu lai...", "");
    
    wifiConnected = false;
    mqttConnected = false;
    
    // Thử kết nối lại
    wifiConnected = setupWiFi();
    if (wifiConnected) {
      mqttConnected = connectToMQTT();
    }
    displayConnectionStatus(wifiConnected, mqttConnected);
    delay(3000); // Hiển thị trạng thái mới trong 3 giây
  }

  // Xử lý các tác vụ từ module ngoại vi (nút bấm, tắt màn hình)
  loopPeripherals(wifiConnected, mqttConnected);

  // Xử lý các tác vụ từ module MQTT (gửi dữ liệu, duy trì kết nối)
  loopMQTT(wifiConnected, mqttConnected);
}
