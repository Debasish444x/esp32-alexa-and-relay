#include "Indicator.h"

long previousMillis = 0;
const long blinkInterval = 100;
int ledState = LOW;

void setupIndicator()
{
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);
}

void blinkLED()
{
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= blinkInterval)
  {
    previousMillis = currentMillis;
    ledState = !ledState;
    digitalWrite(LED_PIN, ledState);
  }
}

void stopBlinking()
{
  digitalWrite(LED_PIN, LOW);
}