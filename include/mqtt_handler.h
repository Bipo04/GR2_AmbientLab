// ================================================================
// FILE HEADER: mqtt_handler.h
// Khai báo các hàm quản lý kết nối MQTT.
// ================================================================

#ifndef MQTT_HANDLER_H
#define MQTT_HANDLER_H

#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include "peripherals.h" // Cần để lấy dữ liệu cảm biến

// Kết nối tới MQTT broker.
// Trả về true nếu thành công.
bool connectToMQTT();

// Vòng lặp MQTT, duy trì kết nối và gửi dữ liệu định kỳ.
// Tham số mqttStatus được truyền bằng tham chiếu (&) để hàm có thể cập nhật trạng thái này.
void loopMQTT(bool wifiStatus, bool &mqttStatus);

#endif