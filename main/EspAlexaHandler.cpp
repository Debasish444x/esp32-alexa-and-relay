#include "EspAlexaHandler.h"
#include "WiFiManager.h"

Espalexa espalexa;

void beginEspAlexa() {
    espalexa.addDevice("relay ten", [](uint8_t b) { relayControl(relay1, b); });
    espalexa.addDevice("relay eleven", [](uint8_t b) { relayControl(relay2, b); });
    espalexa.addDevice("relay twelve", [](uint8_t b) { relayControl(relay3, b); });
    espalexa.addDevice("relay thirteen", [](uint8_t b) { relayControl(relay4, b); });
    espalexa.begin();
    Serial.println("Alexa devices initialized.");
  }
  
  void handleAlexa() {
    if (wifiConnected) {
      espalexa.loop();
    }
  }