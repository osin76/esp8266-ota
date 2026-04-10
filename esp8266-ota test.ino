#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>
#include <WiFiClientSecure.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// LCD
LiquidCrystal_I2C lcd(0x27, 20, 4);

// WiFi
const char* ssid = "Tan Tam Phat";
const char* password = "0888788088";

// Firmware URL
const char* firmware_url = "https://raw.githubusercontent.com/abc/esp8266-ota/main/firmware.bin";

void setup() {
  Serial.begin(115200);

  lcd.init();
  lcd.backlight();

  // ===== KET NOI WIFI =====
  lcd.setCursor(0,0);
  lcd.print("Dang ket noi WiFi");

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // ===== HIEN THI IP =====
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("WiFi OK");

  lcd.setCursor(0,1);
  lcd.print("IP:");
  lcd.print(WiFi.localIP());

  delay(2000);

  // ===== HTTPS CLIENT =====
  WiFiClientSecure client;
  client.setInsecure();

  // ===== CALLBACK HIEN THI % =====
  ESPhttpUpdate.onProgress([](int cur, int total) {
    int percent = (cur * 100) / total;

    lcd.setCursor(0,0);
    lcd.print("Dang update...   ");

    lcd.setCursor(0,1);
    lcd.print("Tien do: ");
    lcd.print(percent);
    lcd.print("%   ");

    Serial.printf("Update: %d%%\n", percent);
  });

  ESPhttpUpdate.onStart([]() {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Bat dau update");
  });

  ESPhttpUpdate.onEnd([]() {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Update xong!");
  });

  ESPhttpUpdate.onError([](int err) {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Update loi!");
  });

  // ===== BAT DAU OTA =====
  t_httpUpdate_return ret = ESPhttpUpdate.update(client, firmware_url);

  // ===== SAU UPDATE =====
  lcd.clear();

  if (ret == HTTP_UPDATE_FAILED) {
    lcd.setCursor(0,0);
    lcd.print("Update LOI");
    delay(3000);
  } 
  else if (ret == HTTP_UPDATE_NO_UPDATES) {
    lcd.setCursor(0,0);
    lcd.print("Khong co ban moi");
    delay(2000);
  }

  // ===== MAN HINH CHAO =====
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Chao Diep");

  lcd.setCursor(0,1);
  lcd.print("He thong OK");

  lcd.setCursor(0,2);
  lcd.print("IP:");
  lcd.print(WiFi.localIP());
}

void loop() {
}