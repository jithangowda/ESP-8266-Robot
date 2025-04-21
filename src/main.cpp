#include <Arduino.h>
#include <ESPConnection.h>

ESPConnection connection("ESP8266_Robot", "robot123");

void setup()
{
  connection.begin();
}

void loop()
{
  connection.listenAndHandle(); // New unified function to manage server + notification
}
