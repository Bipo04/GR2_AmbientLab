# ESP32 Ambient Monitor - Hệ Thống Giám Sát Môi Trường

## Giới Thiệu

ESP32 Ambient Monitor là một dự án IoT sử dụng vi điều khiển ESP32 để giám sát các thông số môi trường xung quanh bao gồm nhiệt độ, độ ẩm, ánh sáng và tia UV. Dữ liệu được thu thập sẽ được hiển thị trên màn hình OLED và gửi lên MQTT broker để theo dõi từ xa.

## Tính Năng

- **Đo Nhiệt Độ & Độ Ẩm**: Sử dụng cảm biến AHT20 để đo chính xác nhiệt độ và độ ẩm không khí
- **Đo Cường Độ Ánh Sáng**: Sử dụng cảm biến TSL2561 để đo ánh sáng môi trường (lux)
- **Đo Tia UV**: Sử dụng cảm biến VEML6075 để đo chỉ số UV (UVA & UVB)
- **Hiển Thị OLED**: Màn hình OLED 128x64 hiển thị dữ liệu thời gian thực
- **Kết Nối WiFi**: Hỗ trợ WiFi Manager để cấu hình WiFi dễ dàng
- **MQTT Protocol**: Gửi dữ liệu lên MQTT broker định kỳ
- **Nút Bấm**: Điều khiển và tương tác với hệ thống
- **Tự Động Kết Nối Lại**: Tự động kết nối lại WiFi và MQTT khi mất kết nối

## Phần Cứng Cần Thiết

- **Vi điều khiển**: ESP32 Development Board
- **Màn hình**: OLED SH1106 128x64 (I2C)
- **Cảm biến**:
  - AHT20 (Nhiệt độ & Độ ẩm)
  - TSL2561 (Cường độ ánh sáng)
  - VEML6075 (Tia UV)

## Thư Viện Sử Dụng

- `Adafruit SH110X` - Điều khiển màn hình OLED
- `Adafruit GFX Library` - Thư viện đồ họa
- `Adafruit AHTX0` - Thư viện cho cảm biến AHT20
- `Adafruit TSL2561` - Thư viện cho cảm biến ánh sáng
- `Adafruit VEML6075 Library` - Thư viện cho cảm biến UV
- `ArduinoJson` - Xử lý dữ liệu JSON
- `WiFiManager` - Quản lý kết nối WiFi
- `PubSubClient` - Giao thức MQTT

## Cấu Trúc Dự Án

```
Esp32/
├── platformio.ini               # Cấu hình PlatformIO
├── README.md                    # File này
├── include/
│   ├── mqtt_handler.h           # Header xử lý MQTT
│   ├── peripherals.h            # Header quản lý thiết bị ngoại vi
│   └── wifi_handler.h           # Header xử lý WiFi
├── src/
│   ├── ESP32_AmbientMonitor.cpp # File chương trình chính
│   ├── mqtt_handler.cpp         # Xử lý kết nối MQTT
│   ├── peripherals.cpp          # Quản lý cảm biến và màn hình
│   └── wifi_handler.cpp         # Xử lý kết nối WiFi
├── lib/                         # Thư viện tùy chỉnh (nếu có)
└── test/                        # Thư mục test
```

## Hướng Dẫn Cài Đặt

### 1. Cài Đặt PlatformIO

Nếu chưa cài đặt PlatformIO, bạn có thể:
- Cài extension PlatformIO IDE cho VS Code

### 2. Clone Dự Án

```bash
git clone <repository-url>
```

### 3. Cấu Hình MQTT

Mở file `mqtt_handler.cpp` và cập nhật thông tin MQTT broker của bạn:
```cpp
// Cấu hình MQTT broker
const char* mqtt_server = "your-mqtt-broker.com";
const int mqtt_port = 1883;
const char* mqtt_user = "your-username";
const char* mqtt_password = "your-password";
```

### 4. Build và Upload

```bash
# Build project
pio run

# Upload lên ESP32
pio run --target upload

# Mở Serial Monitor
pio device monitor
```

## Sơ Đồ Kết Nối

### I2C Bus (SDA, SCL)
- **OLED Display**: SDA → GPIO 21, SCL → GPIO 22
- **AHT20 Sensor**: SDA → GPIO 21, SCL → GPIO 22
- **TSL2561 Sensor**: SDA → GPIO 21, SCL → GPIO 22
- **VEML6075 Sensor**: SDA → GPIO 21, SCL → GPIO 22

### GPIO
- **Nút Bấm**: GPIO 27 → GND (Internal Pull-up)

## Định Dạng Dữ Liệu JSON

Dữ liệu cảm biến được gửi lên MQTT broker dưới dạng JSON:

```json
{
  "temperature": 25.5,
  "humidity": 60.2,
  "luminosity": 350.5,
  "uv_index": 2.3,
  "uva": 1.2,
  "uvb": 0.8
}
```

## Cấu Hình WiFi

Khi khởi động lần đầu hoặc không kết nối được WiFi:
1. ESP32 sẽ tạo một Access Point có tên "ESP32_Setup"
2. Kết nối vào WiFi "ESP32_Setup"
3. Trình duyệt sẽ tự động mở trang cấu hình (hoặc truy cập 192.168.4.1)
4. Nhập thông tin WiFi của bạn và lưu

## Tính Năng Tương Lai

- [ ] Hiện thị chất lượng không khí, bụi pm2.5, pm10

## Tác Giả

Kế thừa và phát triển từ Nguyễn Hữu Dũng

