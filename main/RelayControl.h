#ifndef RELAYCONTROL_H
#define RELAYCONTROL_H

#include <Arduino.h>
#include "StorageManager.h"

extern const int relay1, relay2, relay3, relay4;
void setupRelays();
void relayControl(uint8_t relay, uint8_t brightness);
void restoreRelayStates();
void saveRelayState(int relay, bool state);
void saveInternetState(bool state);

#endif