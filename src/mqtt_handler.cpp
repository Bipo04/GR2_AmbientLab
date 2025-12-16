// ================================================================
// FILE IMPLEMENTATION: mqtt_handler.cpp
// Logic kết nối, gửi dữ liệu, và duy trì kết nối MQTT.
// ================================================================

#include "mqtt_handler.h"

const char* mqttServer = "mqtt.toolhub.app";
const int mqttPort = 1883;
const char* mqttUser = "username";
const char* mqttPassword = "password";
const char* mqttTopic = "topic";

// Các biến toàn cục cho MQTT
WiFiClient espClient;
PubSubClient client(espClient);

unsigned long lastMqttUpdate = 0;
const unsigned long mqttInterval = 5000;

// Hàm gửi dữ liệu cảm biến lên MQTT
void sendDataToMqtt() {
  if (WiFi.status() != WL_CONNECTED || !client.connected()) return;

  String payload = getSensorJson(); // Lấy chuỗi JSON từ module ngoại vi
  
  Serial.println("Sending MQTT: " + payload);
  if (client.publish(mqttTopic, payload.c_str())) {
    Serial.println("Gửi dữ liệu thành công!");
  } else {
    Serial.println("Gửi dữ liệu thất bại!");
  }
}

// Kết nối MQTT
bool connectToMQTT() {
  // espClient.setInsecure(); // Tắt kiểm tra certificate
  client.setServer(mqttServer, mqttPort);

  Serial.print("Đang kết nối MQTT...");
  displayMessage("Dang ket noi MQTT...", "", "");

  int attempts = 0;
  while (!client.connected() && attempts < 3) {
    String clientId = "ESP32_Sensor_Hub_" + String(random(0xffff), HEX);
    if (client.connect(clientId.c_str(), mqttUser, mqttPassword)) {
      Serial.println(" Thanh cong!");
      return true;
    } else {
      Serial.print(".");
      attempts++;
      delay(2000);
    }
  }
  Serial.println(" That bai!");
  return false;
}

// Vòng lặp MQTT
void loopMQTT(bool wifiStatus, bool &mqttStatus) {
  if (!wifiStatus) return;

  // Nếu không có kết nối, thử kết nối lại định kỳ
  if (!client.connected()) {
    // Nếu trước đó đang kết nối, in ra thông báo
    if (mqttStatus == true) {
      Serial.println("Mất kết nối MQTT, đang thử kết nối lại...");
      mqttStatus = false; // Cập nhật trạng thái ngay lập tức
    }
    
    // Thử kết nối lại mỗi 5 giây
    if (millis() - lastMqttUpdate > 5000) {
      lastMqttUpdate = millis(); // Đặt lại bộ đếm thời gian
      mqttStatus = connectToMQTT();
    }
  }

  // Nếu đang kết nối, thì duy trì và gửi dữ liệu
  if (mqttStatus) {
    client.loop(); // Duy trì kết nối

    // Gửi dữ liệu định kỳ
    if (millis() - lastMqttUpdate >= mqttInterval) {
      lastMqttUpdate = millis();
      sendDataToMqtt();
    }
  }
}
