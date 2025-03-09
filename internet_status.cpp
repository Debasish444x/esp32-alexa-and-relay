#ifdef ARDUINO_ARCH_ESP32
#include <WiFi.h>
#include <Preferences.h>
#else
#include <ESP8266WiFi.h>
#include <EEPROM.h>
#endif
#include <Espalexa.h>
#include <HTTPClient.h>

// WiFi Credentials
const char* ssid = "WAKANDA_FOREVER";
const char* password = "Bharati8640";

// Relay Pins
const int relay1 = 23;
const int relay2 = 22;
const int relay3 = 21;
const int relay4 = 19;
const int led = 2;  // LED for indication

unsigned long previousMillis = 0;
const long blinkInterval = 500;  // LED blink interval
int ledState = LOW;
const long internetCheckInterval = 30000;  // 30 seconds
unsigned long lastCheckTime = 0;
// Device Names
const char* device_1_name = "exaust fan";
const char* device_2_name = "kitchen light";
const char* device_3_name = "dinning light";
const char* device_4_name = "dinning fan";

boolean checkInternet = false;
boolean wifiConnected = false;
boolean previousInternetState = false;

Espalexa espalexa;  // Alexa Instance

#ifdef ARDUINO_ARCH_ESP32
Preferences preferences;
#else
#define EEPROM_SIZE 5
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
void beginEspAlexa();
void checkInternetAccess();
void blinkLED();
void saveInternetState(bool state);
bool restoreInternetState();


void setup() {
  Serial.begin(115200);

  // Initialize storage
#ifdef ARDUINO_ARCH_ESP32
  preferences.begin("relayState", false);
  previousInternetState = preferences.getBool("internetState", false);
#else
  EEPROM.begin(EEPROM_SIZE);
  previousInternetState = EEPROM.read(4);
#endif

  // Setup relay pins
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);
  pinMode(relay4, OUTPUT);
  pinMode(led, OUTPUT);
  digitalWrite(led, HIGH);

  // Restore last saved states
  restoreRelayStates();

  // Connect to WiFi
  setupWifi();
  if (wifiConnected) {
    beginEspAlexa();
    Serial.println("From Setup: All relays added to Alexa. Now checking internet access");
    checkInternetAccess();

  } else {
    Serial.println("WiFi NOT connected! Alexa control disabled, but relays will still work manually.");
  }
}

void loop() {
  checkWifiStatus();
  if (millis() - lastCheckTime > internetCheckInterval) {
    lastCheckTime = millis();
    checkInternetAccess();
    Serial.println("From LOOP: INTERNET STATUS(_EVERY_30_SEC_WILL_CHECK_FOR_INTERNET!): " + String(checkInternet));
  }
  if (wifiConnected) {
    espalexa.loop();  // Keep Alexa running
  } else {
    setupWifi();  // Attempt to reconnect WiFi
    Serial.println("WIFI STATUS: " + String(wifiConnected));
  }

  delay(1);  // Prevent excessive loops
}

void beginEspAlexa() {
  espalexa.addDevice(device_1_name, relay1Control);
  espalexa.addDevice(device_2_name, relay2Control);
  espalexa.addDevice(device_3_name, relay3Control);
  espalexa.addDevice(device_4_name, relay4Control);
  espalexa.begin();
  Serial.println("Alexa devices initialized.");
}

// **WiFi & Internet Monitoring**
void checkWifiStatus() {
  if (WiFi.status() == WL_CONNECTED) {
    wifiConnected = true;
  } else {
    Serial.println("WiFi lost! Reconnecting...");
    wifiConnected = false;
    setupWifi();  // Auto-reconnect
  }
}

void setupWifi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
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
    digitalWrite(led, LOW);
  } else {
    Serial.println("Failed to connect to WiFi.");
    wifiConnected = false;
    blinkLED();
  }
}

// **Check Internet Connection**
void checkInternetAccess() {
  HTTPClient http;
  http.begin("http://clients3.google.com/generate_204");
  int httpCode = http.GET();
  http.end();

  previousInternetState = checkInternet;
  checkInternet = (httpCode > 0);  // Any valid response means internet is available

  saveInternetState(checkInternet);

  if (!previousInternetState && checkInternet) {
    Serial.println("Internet restored! Restarting Alexa...");
    setupWifi(); //re-init setup ensures smooth internet connectivity
  }

  Serial.println("Previous internet state: " + String(previousInternetState));
  Serial.println("Current internet state: " + String(checkInternet));
}

void saveInternetState(bool state) {
#ifdef ARDUINO_ARCH_ESP32
  preferences.putBool("internetState", state);
#else
  EEPROM.write(4, state);
  EEPROM.commit();
#endif
}

// **Blink LED**
void blinkLED() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= blinkInterval) {
    previousMillis = currentMillis;
    ledState = !ledState;
    digitalWrite(led, ledState);
  }
}

// **Relay Control**
void relayControl(uint8_t relay, uint8_t brightness) {
  bool state = brightness > 0 ? LOW : HIGH;
  digitalWrite(relay, state);
  saveRelayState(relay, state);
}

void restoreRelayStates() { //future improvement: sorten this code
#ifdef ARDUINO_ARCH_ESP32
  digitalWrite(relay1, preferences.getBool("relay1", HIGH));
  digitalWrite(relay2, preferences.getBool("relay2", HIGH));
  digitalWrite(relay3, preferences.getBool("relay3", HIGH));
  digitalWrite(relay4, preferences.getBool("relay4", HIGH));
#else
  digitalWrite(relay1, EEPROM.read(0) ? LOW : HIGH);
  digitalWrite(relay2, EEPROM.read(1) ? LOW : HIGH);
  digitalWrite(relay3, EEPROM.read(2) ? LOW : HIGH);
  digitalWrite(relay4, EEPROM.read(3) ? LOW : HIGH);
#endif
}

void saveRelayState(int relay, bool state) { //future improvement: sorten this code
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
//future improvement: can be sorten
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