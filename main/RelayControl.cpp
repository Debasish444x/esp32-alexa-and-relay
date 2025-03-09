#include "RelayControl.h"

const int relay1 = 23;
const int relay2 = 22;
const int relay3 = 21;
const int relay4 = 19;
Preferences preferences;
void setupRelays() {
  preferences.begin("relayState", false);
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);
  pinMode(relay4, OUTPUT);
  restoreRelayStates();
}

void relayControl(uint8_t relay, uint8_t brightness) {
  bool state = brightness > 0 ? LOW : HIGH;
  digitalWrite(relay, state);
  saveRelayState(relay, state);
}

void restoreRelayStates() {
  digitalWrite(relay1, preferences.getBool("relay1", HIGH));
  digitalWrite(relay2, preferences.getBool("relay2", HIGH));
  digitalWrite(relay3, preferences.getBool("relay3", HIGH));
  digitalWrite(relay4, preferences.getBool("relay4", HIGH));
}

void saveRelayState(int relay, bool state) {
  preferences.putBool(relay == relay1 ? "relay1" : relay == relay2 ? "relay2"
                                                 : relay == relay3 ? "relay3"
                                                                   : "relay4",
                      state);
}