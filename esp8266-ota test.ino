#include <ESP8266WiFi.h>
#include <ESP8266httpUpdate.h>
#include <WiFiClientSecure.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// LCD 2004A
LiquidCrystal_I2C lcd(0x27, 20, 4);

// WiFi
const char* ssid = "Tan Tam Phat";
const char* password = "0888788088";

// Link firmware (bạn thay bằng link GitHub của bạn)
const char* firmware_url = "https://github.com/osin76/esp8266-ota/blob/main/esp8266-ota%20test.ino";

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
  }

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("WiFi OK");

  lcd.setCursor(0,1);
  lcd.print("IP:");
  lcd.print(WiFi.localIP());

  delay(2000);

  // ===== CLIENT HTTPS =====
  WiFiClientSecure client;
  client.setInsecure();

  // ===== HIEN THI % UPDATE =====
  ESPhttpUpdate.onProgress([](int cur, int total) {
    int percent = (cur * 100) / total;

    lcd.setCursor(0,0);
    lcd.print("Dang update...   ");

    lcd.setCursor(0,1);
    lcd.print("Tien do: ");
    lcd.print(percent);
    lcd.print("%   ");
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

  // ===== BAT DAU UPDATE =====
  t_httpUpdate_return ret = ESPhttpUpdate.update(client, firmware_url);

  // ===== SAU KHI XONG =====
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Chao Diep");

  lcd.setCursor(0,1);
  lcd.print("He thong OK");

  lcd.setCursor(0,2);
  lcd.print("IP:");
  lcd.print(WiFi.localIP());
}

void loop() {}
