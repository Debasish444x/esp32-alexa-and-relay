#ifdef ARDUINO_ARCH_ESP32
#include <WiFi.h>
#include <Preferences.h>  // ESP32 Preferences Storage
#else
#include <ESP8266WiFi.h>
#include <EEPROM.h>  // EEPROM for ESP8266
#endif
#include <Espalexa.h>

// WiFi Credentials
const char* ssid = "AVENGERS_ASSAMBLE"; //Your ssid
const char* password = "....WANDA_VISION"; // your password

// Relay Pins
const int relay1 = 23;
const int relay2 = 22;
const int relay3 = 21;
const int relay4 = 19;
//for led blink
const int led = 2;
unsigned long previousMillis = 0;
const long blinkInterval = 25;
int ledState = LOW;
// Device Names
const char* device_1_name = "relay one";
const char* device_2_name = "relay two";
const char* device_3_name = "relay three";
const char* device_4_name = "relay four";

// WiFi Connection Status
boolean wifiConnected = false;

// Espalexa Instance
Espalexa espalexa;

// Storage Object
#ifdef ARDUINO_ARCH_ESP32
Preferences preferences;
#else
#define EEPROM_SIZE 4
#endif

// Function Prototypes
void relayControl(uint8_t relay, uint8_t brightness);
void relay1Control(uint8_t brightness);
void relay2Control(uint8_t brightness);
void relay3Control(uint8_t brightness);
void relay4Control(uint8_t brightness);
void restoreRelayStates();
void saveRelayState(int relay, bool state);
void checkWifiStatus();
void setupWifi();

void setup() {
  Serial.begin(115200);

// Initialize Storage
#ifdef ARDUINO_ARCH_ESP32
  preferences.begin("relayState", false);
#else
  EEPROM.begin(EEPROM_SIZE);
#endif

  // Setup Relay Pins
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);
  pinMode(relay4, OUTPUT);
  pinMode(led,OUTPUT);
  digitalWrite(led,HIGH);
  // Restore Last Saved State
  restoreRelayStates();

  // Try Connecting to WiFi
  setupWifi();

  // Add Relays to Espalexa (if WiFi is connected)
  if (wifiConnected) {
    espalexa.addDevice(device_1_name, relay1Control);
    espalexa.addDevice(device_2_name, relay2Control);
    espalexa.addDevice(device_3_name, relay3Control);
    espalexa.addDevice(device_4_name, relay4Control);
    espalexa.begin();
    Serial.println("All relays added to Alexa.");
  } else {
    Serial.println("WiFi NOT connected! Alexa control disabled, but relays will still work manually.");
  }
}

void loop() {
  checkWifiStatus();
  if (wifiConnected) {
    espalexa.loop();  // Run Alexa control when WiFi is available
  } else {
    setupWifi();
  }

  delay(1);
}

// **Ensure WiFi Stays Connected (Retries Every 5 Secs)**
void checkWifiStatus() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi Lost! Reconnecting...");
    wifiConnected = false;
  } else {
    wifiConnected = true;
  }
}



// **Setup WiFi Connection (Keeps Retrying Forever)**
void setupWifi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("\nConnecting to WiFi...");
  int reTry = 0;
  while (WiFi.status() != WL_CONNECTED && reTry < 15) {
    blinkLED();
    Serial.print(".");
    delay(2000);
    reTry++;
  };
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi Connected!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
    wifiConnected = true;
    digitalWrite(led,LOW);
  } else {
    Serial.println("\nFiled to connect with wifi");
    wifiConnected = false;
    blinkLED();
  }
}

// **Generic Relay Control Function**
void relayControl(uint8_t relay, uint8_t brightness) {
  bool state = brightness > 0 ? LOW : HIGH;  // Relays are active LOW
  digitalWrite(relay, state);
  saveRelayState(relay, state);
}

void blinkLED() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= blinkInterval) {
    previousMillis = currentMillis;
    ledState = !ledState; // Toggle LED state
    digitalWrite(led, ledState);
  }
}

// **Individual Relay Control Functions**
void relay1Control(uint8_t brightness) {
  relayControl(relay1, brightness);
}
void relay2Control(uint8_t brightness) {
  relayControl(relay2, brightness);
}
void relay3Control(uint8_t brightness) {
  relayControl(relay3, brightness);
}
void relay4Control(uint8_t brightness) {
  relayControl(relay4, brightness);
}

// **Restore Last Saved Relay States**
void restoreRelayStates() {
#ifdef ARDUINO_ARCH_ESP32
  digitalWrite(relay1, preferences.getBool("relay1", HIGH));
  digitalWrite(relay2, preferences.getBool("relay2", HIGH));
  digitalWrite(relay3, preferences.getBool("relay3", HIGH));
  digitalWrite(relay4, preferences.getBool("relay4", HIGH));
#else
  digitalWrite(relay1, EEPROM.read(0) == 1 ? LOW : HIGH);
  digitalWrite(relay2, EEPROM.read(1) == 1 ? LOW : HIGH);
  digitalWrite(relay3, EEPROM.read(2) == 1 ? LOW : HIGH);
  digitalWrite(relay4, EEPROM.read(3) == 1 ? LOW : HIGH);
#endif
  Serial.println("Relay states restored.");
}

// **Save Relay State**
void saveRelayState(int relay, bool state) {
#ifdef ARDUINO_ARCH_ESP32
  preferences.putBool(relay == relay1 ? "relay1" : relay == relay2 ? "relay2"
                                                 : relay == relay3 ? "relay3"
                                                                   : "relay4",
                      state);
#else
  EEPROM.write(relay == relay1 ? 0 : relay == relay2 ? 1
                                   : relay == relay3 ? 2
                                                     : 3,
               state ? 1 : 0);
  EEPROM.commit();
#endif
}
