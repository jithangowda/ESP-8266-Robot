#include <Arduino.h>
#include <Arduino.h>
#include <WiFiUdp.h>
#include <ESPConnection.h>

ESPConnection connection("ESP8266_Robot", "robot123");

void setup()
{
  connection.begin();
}

void loop()
{
  connection.listenForServer();
}