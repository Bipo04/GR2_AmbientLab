// ================================================================
// FILE IMPLEMENTATION: peripherals.cpp
// Logic khởi tạo và điều khiển màn hình, cảm biến, nút bấm.
// ================================================================

#include "peripherals.h"

// Khởi tạo các đối tượng thiết bị
Adafruit_SH1106G display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Adafruit_AHTX0 aht;
Adafruit_TSL2561_Unified tsl = Adafruit_TSL2561_Unified(LUMINOSITY_ADDRESS, 12345);
Adafruit_VEML6075 uv = Adafruit_VEML6075();

// Biến cục bộ của module
int sensorIndex = -1;
unsigned long lastButtonPress = 0;
unsigned long lastDisplayTime = 0;
const unsigned long displayTimeout = 5000;
bool displayOn = false;

// Hàm hiển thị dữ liệu cảm biến cụ thể 
void displaySensorData(int index, bool wifiStatus, bool mqttStatus) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(0, 0);

  if (index == 0) {
    sensors_event_t humidity, temp; aht.getEvent(&humidity, &temp);
    display.println("AHT30 - Temp/Humidity");
    display.println("");
    display.print("Nhiet do: "); display.print(temp.temperature, 1); 
    display.print(" "); display.print(char(167)); display.println("C");
    display.print("Do am   : "); display.print(humidity.relative_humidity, 1); display.println(" %");
  } else if (index == 1) {
    uint16_t broadband, infrared; tsl.getLuminosity(&broadband, &infrared);
    uint32_t lux = tsl.calculateLux(broadband, infrared);
    display.println("TSL2561 - Anh sang");
    display.println("");
    display.print("Broadband: "); display.println(broadband);
    display.print("Infrared : "); display.println(infrared);
    display.print("Lux      : "); display.println(lux);
  } else if (index == 2) {
    float uva = uv.readUVA();
    float uvb = uv.readUVB();
    float uvi = uv.readUVI();
    display.println("VEML6075 - Tia UV");
    display.println("");
    display.print("UVA: "); display.println(uva, 2);
    display.print("UVB: "); display.println(uvb, 2);
    display.print("UVI: "); display.println(uvi, 2);
  }

  display.setCursor(0, 56);
  if (wifiStatus && mqttStatus) display.print("WiFi + MQTT: OK");
  else if (wifiStatus) display.print("WiFi: OK, MQTT: X");
  else display.print("WiFi: X, MQTT: X");
  display.display();
}

// Khởi tạo
void setupPeripherals() {
  Wire.begin();
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  if (!display.begin(SCREEN_ADDRESS, true)) Serial.println("Khong tim thay OLED!");
  if (!aht.begin()) Serial.println("Khong tim thay AHT30!");
  if (!tsl.begin()) Serial.println("Khong tim thay TSL2561!");
  else {
    tsl.enableAutoRange(true);
    tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_402MS);
  }
  if (!uv.begin()) Serial.println("Khong tim thay VEML6075!");
}

// Vòng lặp
void loopPeripherals(bool wifiStatus, bool mqttStatus) {
  unsigned long currentTime = millis();

  if (digitalRead(BUTTON_PIN) == LOW && currentTime - lastButtonPress > 400) {
    lastButtonPress = currentTime;
    sensorIndex = (sensorIndex + 1) % 3;
    displayOn = true;
    lastDisplayTime = currentTime;
    displaySensorData(sensorIndex, wifiStatus, mqttStatus);
  }

  if (displayOn && (currentTime - lastDisplayTime > displayTimeout)) {
    displayOn = false;
    display.clearDisplay();
    display.display();
  }
}

// Hàm hiển thị thông báo
void displayMessage(const char* line1, const char* line2, const char* line3) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(0, 0);
  display.println(line1);
  display.println(line2);
  display.println(line3);
  display.display();
}

// Hàm hiển thị trạng thái kết nối
void displayConnectionStatus(bool wifiStatus, bool mqttStatus) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(0, 0);
  display.println("Trang thai ket noi:");
  display.print("WiFi: "); display.println(wifiStatus ? "Da ket noi" : "Khong ket noi");
  display.print("MQTT: "); display.println(mqttStatus ? "Da ket noi" : "Khong ket noi");
  display.println("\nNhan nut de xem");
  display.println("du lieu cam bien.");
  display.display();
}

// Lấy chuỗi JSON (ĐÃ CẬP NHẬT)
String getSensorJson() {
  sensors_event_t humidity, temp; aht.getEvent(&humidity, &temp);
  uint16_t broadband, infrared; tsl.getLuminosity(&broadband, &infrared);
  uint32_t lux = tsl.calculateLux(broadband, infrared);
  float uva = uv.readUVA();
  float uvb = uv.readUVB();
  float uvi = uv.readUVI();

  // Sử dụng ArduinoJson để tạo payload một cách an toàn, tránh phân mảnh bộ nhớ
  StaticJsonDocument<384> doc; // Tăng kích thước để đảm bảo đủ chỗ

  doc["temperature"] = String(temp.temperature, 2);
  doc["humidity"] = String(humidity.relative_humidity, 2);
  doc["broadband"] = broadband;
  doc["infrared"] = infrared;
  doc["lux"] = lux;
  doc["UVA"] = String(uva, 2);
  doc["UVB"] = String(uvb, 2);
  doc["UVI"] = String(uvi, 2);

  String payload;
  serializeJson(doc, payload); // Chuyển đổi đối tượng JSON thành chuỗi
  return payload;
}
