// ================================================================
// FILE IMPLEMENTATION: wifi_handler.cpp
// Chứa logic kết nối nhiều mạng Wi-Fi và quản lý bằng WiFiManager.
// ================================================================

#include "wifi_handler.h"

// Biến cờ cục bộ của module này
bool shouldSaveConfig = false;

// Callback function sẽ được gọi khi WifiManager vào chế độ cấu hình
void saveConfigCallback()
{
  Serial.println("Đã kết nối Wi-Fi mới, chuẩn bị lưu cấu hình.");
  shouldSaveConfig = true;
}

// Hàm lưu/cập nhật danh sách mạng
void addNewWifiConfig()
{
  Serial.println("Đang cập nhật danh sách mạng Wi-Fi...");
  String newSsid = WiFi.SSID();
  String newPass = WiFi.psk();
  StaticJsonDocument<1024> doc;
  JsonArray networks;

  if (LittleFS.exists(CONFIG_FILE))
  {
    File configFile = LittleFS.open(CONFIG_FILE, "r");
    if (configFile)
    {
      if (deserializeJson(doc, configFile) == DeserializationError::Ok)
      {
        networks = doc.as<JsonArray>();
      }
      configFile.close();
    }
  }
  else
  {
    networks = doc.to<JsonArray>();
  }

  for (int i = 0; i < networks.size(); i++)
  {
    if (networks[i]["ssid"] == newSsid)
    {
      networks.remove(i);
      break;
    }
  }

  while (networks.size() >= MAX_NETWORKS)
  {
    networks.remove(0);
  }

  JsonObject newNetwork = networks.createNestedObject();
  newNetwork["ssid"] = newSsid;
  newNetwork["pass"] = newPass;

  File configFile = LittleFS.open(CONFIG_FILE, "w");
  if (configFile)
  {
    serializeJson(doc, configFile);
    configFile.close();
    Serial.println("Đã lưu danh sách mạng thành công!");
  }
}

// Hàm thử kết nối với các mạng đã lưu
bool connectToSavedNetworks()
{
  if (!LittleFS.exists(CONFIG_FILE))
    return false;
  File configFile = LittleFS.open(CONFIG_FILE, "r");
  if (!configFile)
    return false;

  StaticJsonDocument<1024> doc;
  if (deserializeJson(doc, configFile) != DeserializationError::Ok)
  {
    configFile.close();
    return false;
  }
  configFile.close();

  JsonArray networks = doc.as<JsonArray>();
  if (networks.size() == 0) return false;

  Serial.println("Bắt đầu quét các mạng đã lưu...");
  displayMessage("Dang quet mang...", "", "");

  for (int i = networks.size() - 1; i >= 0; i--)
  {
    String ssid = networks[i]["ssid"];
    String pass = networks[i]["pass"];

    Serial.print("Dang thu: ");
    Serial.println(ssid);
    displayMessage("Dang thu ket noi:", ssid.c_str(), "");

    WiFi.begin(ssid.c_str(), pass.c_str());
    int attempt = 0;
    while (WiFi.status() != WL_CONNECTED && attempt++ < 20)
    {
      delay(250);
      Serial.print(".");
    }

    if (WiFi.status() == WL_CONNECTED)
    {
      Serial.println("\n=> Ket noi thanh cong!");
      if (i < networks.size() - 1)
      {
        Serial.println("   Cap nhat danh sach uu tien...");

        // Lấy thông tin mạng thành công
        String successSsid = networks[i]["ssid"];
        String successPass = networks[i]["pass"];

        // Xóa nó khỏi vị trí hiện tại
        networks.remove(i);

        // Thêm lại vào cuối
        JsonObject newLastNetwork = networks.createNestedObject();
        newLastNetwork["ssid"] = successSsid;
        newLastNetwork["pass"] = successPass;

        // Ghi lại file cấu hình
        File configFileWrite = LittleFS.open(CONFIG_FILE, "w");
        if (configFileWrite)
        {
          serializeJson(doc, configFileWrite);
          configFileWrite.close();
          Serial.println("   => Da cap nhat uu tien.");
        }
      }
      return true;
    }
    else
    {
      Serial.println("\n=> That bai.");
      WiFi.disconnect(true);
      delay(100);
    }
  }
  return false;
}

// Hàm chính của module
bool setupWiFi()
{
  // Khởi tạo LittleFS
  if (!LittleFS.begin(true))
  {
    Serial.println("Lỗi nghiêm trọng! Không thể khởi tạo LittleFS.");
    displayMessage("LOI HE THONG FILE", "Vui long kiem tra", "Va khoi dong lai");
    return false;
  }

  // Thử kết nối mạng đã lưu
  if (connectToSavedNetworks())
  {
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    return true;
  }

  // Nếu không thành công, mở portal cấu hình
  Serial.println("Chuyển sang chế độ cấu hình.");
  String apName = "ESP32-Sensor-Config";
  displayMessage("Che do cau hinh:", "SSID:", apName.c_str());

  WiFiManager wm;
  wm.setSaveConfigCallback(saveConfigCallback);
  wm.setConfigPortalTimeout(180); // 3 phút

  if (!wm.autoConnect(apName.c_str()))
  {
    Serial.println("Ket noi that bai. Khoi dong lai...");
    displayMessage("Cau hinh that bai", "Dang khoi dong lai...", "");
    delay(3000);
    ESP.restart();
  }
  else
  {
    Serial.println("Ket noi thanh cong qua portal!");
    if (shouldSaveConfig)
    {
      addNewWifiConfig();
    }
    return true;
  }
  return false; // Trường hợp dự phòng
}