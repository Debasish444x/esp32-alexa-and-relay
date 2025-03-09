#ifndef WIFIMANAGER_H
#define WIFIMANAGER_H

#include <WiFi.h>
#include <HTTPClient.h>
#include "Indicator.h"
#include "StorageManager.h"
extern boolean checkInternet;
extern boolean wifiConnected;

void setupWifi();
void checkWifiStatus();
void checkInternetAccess();
void handleInternetCheck();
void saveInternetState(bool state);

#endif