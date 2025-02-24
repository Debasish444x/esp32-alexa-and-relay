# ESP32 Relay Control with Alexa & WiFi Auto-Reconnect

## 📌 Overview
This project enables **ESP32** to control **4 relays** using **Amazon Alexa**, even when WiFi is disconnected. It also supports **automatic relay state restoration** after power loss and continuously retries to connect to WiFi until it is available.

## 🚀 Features
- ✅ **Alexa Voice Control** (Using Espalexa Library)
- ✅ **Persistent Relay State Storage** (Using Preferences/EEPROM)
- ✅ **WiFi Auto-Reconnect** (Keeps retrying all day!)
- ✅ **LED Indicator** (Blinks while connecting, stays ON if WiFi fails)

## 🛠️ Hardware Requirements
- **ESP32 WROOM-32 Module**
- **4-Channel 5V Relay Module**
- **LED for WiFi status (Connected to GPIO2)**
- **Power Supply (12V 1A + DC-DC 5V Regulator)**

## 🔌 Wiring Diagram
| ESP32 Pin | Relay Module | Other |
|-----------|-------------|-------|
| GPIO 23   | Relay 1     |       |
| GPIO 22   | Relay 2     |       |
| GPIO 21   | Relay 3     |       |
| GPIO 19   | Relay 4     |       |
| GPIO 2    | LED (WiFi)  |       |

## 📦 Required Libraries
Before uploading the code, install these libraries in **Arduino IDE**:

1. [WiFi](https://www.arduino.cc/en/Reference/WiFi)
2. [Preferences](https://github.com/espressif/arduino-esp32/tree/master/libraries/Preferences) (For ESP32 storage)
3. [Espalexa](https://github.com/Aircoookie/Espalexa) (For Alexa Integration)

## 📜 Installation & Setup
1. **Download & Install the Required Libraries** (See above)
2. **Modify `espalexa.h` to Fix Multiple Callback Issue** (See below)
3. **Upload `esp32_relay_control.ino` to ESP32**
4. **Connect ESP32 to Alexa via the Alexa App**
5. **Control Relays with Voice Commands**

## 🔧 Fixing the Espalexa Callback Issue
**Problem:** If you have multiple devices, they may all trigger the first callback due to a bug in `espalexa.h`.

**Solution:** Modify the unique ID generation line in `espalexa.h`:

Replace:
```cpp
sprintf_P(out, PSTR("%02X:%02X:%02X:%02X:%02X:%02X:00:11-%02X"), mac[0], mac[1], mac[2], mac[3], mac[4], mac[5], idx);
```
With:
```cpp
sprintf_P(out, PSTR("%02X:%02X:%02X:%02X:%02X:%02X-%02X-00:11"), mac[0], mac[1], mac[2], mac[3], mac[4], mac[5], idx);
```
📌 **File to Modify:** `espalexa.h` (Located in the ESPAlexa library folder)

🔗 **Original Discussion:** Arduino Forum - "ESPAlexa Only One Callback Working"

## 🛠️ Functionality Breakdown
- **setupWifi()** → Connects ESP32 to WiFi & blinks LED until successful.
- **checkWifiStatus()** → Ensures ESP remains connected, retries every 5 seconds.
- **restoreRelayStates()** → Loads last relay states from memory.
- **relayControl()** → Generic function to control relays.
- **blinkLED()** → Blinks LED while WiFi is connecting.

## 🎤 Alexa Voice Commands
| Command               | Action               |
|----------------------|---------------------|
| "Alexa, turn on relay one" | Turns on Relay 1 |
| "Alexa, turn off relay two" | Turns off Relay 2 |
| "Alexa, turn on relay three" | Turns on Relay 3 |
| "Alexa, turn off relay four" | Turns off Relay 4 |

## 📌 Notes
- Ensure your ESP32 and Alexa device are on the **same WiFi network**.
- If Alexa doesn't discover the device, **reboot ESP32 and retry**.
- If WiFi disconnects, ESP32 **will automatically reconnect without rebooting**.

## 📝 License
This project is open-source and free to use! 🚀

