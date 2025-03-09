#include "WiFiManager.h"
#include "RelayControl.h"
#include "EspAlexaHandler.h"
#include "Indicator.h"

void setup() {
  Serial.begin(115200);

  setupRelays();
  setupIndicator();
  setupWifi();
  beginEspAlexa();

  Serial.println("From Setup: All relays added to Alexa. Now checking internet access");
  checkInternetAccess();
}

void loop() {
  checkWifiStatus();
  handleInternetCheck();
  handleAlexa();
}
