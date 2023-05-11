#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#define batteryRelay D2
#define vccSrcRelay D7
#define voltageSensor A0
#define statusLED D5

int incoming = 0;
int voltage = 0;

bool isHome()
{
  int numberOfTries = 0;

  WiFi.disconnect();
  delay(2000);
  WiFi.begin("Xena", "redesemfio1");
  while (WiFi.status() != WL_CONNECTED && numberOfTries < 150)
  {
    delay(100);
    Serial.print(".");
    numberOfTries++;
  }
  if (WiFi.status() == WL_CONNECTED)
  {
    return true;
  }
  else
  {
    return false;
  }
}

void blinkLed(int blinkQuantity, int blinkSpacing)
{
  int counter = 0;
  while (counter < blinkQuantity)
  {
    counter++;
    digitalWrite(statusLED, HIGH);
    delay(blinkSpacing);
    digitalWrite(statusLED, LOW);
    delay(blinkSpacing);
  }
}

void initialize()
{
  digitalWrite(statusLED, HIGH);
  delay(5000);
  digitalWrite(batteryRelay, HIGH);
  delay(2000);
  digitalWrite(vccSrcRelay, HIGH);
  delay(200);
  digitalWrite(statusLED, LOW);
}

void setup()
{

  pinMode(batteryRelay, OUTPUT);
  pinMode(vccSrcRelay, OUTPUT);
  pinMode(voltageSensor, INPUT);
  pinMode(statusLED, OUTPUT);

  digitalWrite(statusLED, LOW);
  digitalWrite(batteryRelay, LOW);
  digitalWrite(voltageSensor, LOW);

  Serial.begin(9600);

  initialize();
  delay(300000);
}

void loop()
{
  voltage = analogRead(voltageSensor);
  Serial.print("Voltage: ");
  Serial.println(voltage);
  if (isHome())
  {
    Serial.println("Is Home");
    blinkLed(5, 200);

    digitalWrite(batteryRelay, LOW);
    delay(100);
    digitalWrite(vccSrcRelay, LOW);

    delay(300000);

    digitalWrite(batteryRelay, HIGH);
    digitalWrite(vccSrcRelay, HIGH);
  }
  if (voltage <= 800)
  {
    Serial.println("Low Voltage...");
    Serial.print(voltage);
    blinkLed(40, 80);

    digitalWrite(batteryRelay, LOW);
    delay(100);
    digitalWrite(vccSrcRelay, LOW);
    delay(100000);
    ESP.restart();
  }
  delay(60000);
}