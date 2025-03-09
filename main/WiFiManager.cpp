#include "WiFiManager.h"
boolean checkInternet = false;
boolean previousInternetState = false;
boolean wifiConnected = false;

unsigned long lastCheckTime = 0;
const long internetCheckInterval = 6000;

void setupWifi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin("WAKANDA_FOREVER", "********");
  Serial.println("Connecting to WiFi...");

  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 15) {
    blinkLED();
    delay(2000);
    attempts++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("WiFi Connected!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
    wifiConnected = true;
    stopBlinking();
  } else {
    Serial.println("Failed to connect to WiFi.");
    wifiConnected = false;
    blinkLED();
  }
}

void checkWifiStatus() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi lost! Reconnecting...");
    wifiConnected = false;
    setupWifi();
  } else {
    wifiConnected = true;
  }
}



void checkInternetAccess() {
  HTTPClient http;
  http.begin("http://clients3.google.com/generate_204");
  int httpCode = http.GET();
  http.end();

  previousInternetState = checkInternet;
  checkInternet = (httpCode > 0);  // Any valid response means internet is available

  saveInternetState(checkInternet);

  if (!previousInternetState && checkInternet) {
    Serial.println("Internet restored! Re init wifi-setup...");
    setupWifi(); //re-init setup ensures smooth internet connectivity
  }

  Serial.println("Previous internet state: " + String(previousInternetState));
  Serial.println("Current internet state: " + String(checkInternet));
}
void handleInternetCheck() {
  if (millis() - lastCheckTime > internetCheckInterval) {
    lastCheckTime = millis();
    checkInternetAccess();
  }
}

void saveInternetState(bool state) {
  #ifdef ARDUINO_ARCH_ESP32
    preferences.putBool("internetState", state);
  #else
    EEPROM.write(4, state);
    EEPROM.commit();
  #endif
  }
