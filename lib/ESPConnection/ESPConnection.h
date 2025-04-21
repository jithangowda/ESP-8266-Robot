#ifndef ESP_CONNECTION_H
#define ESP_CONNECTION_H

#include <ESP8266WiFi.h>
#include <WiFiManager.h>
#include <WiFiUdp.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

class ESPConnection
{
public:
    ESPConnection(const char *apName, const char *apPassword);
    void begin();
    void listenAndHandle();

private:
    const char *_apName;
    const char *_apPassword;
    WiFiUDP udp, notifyUdp;
    const int UDP_PORT = 4210;
    const int NOTIFY_PORT = 4213;
    bool serverFound = false;
    bool allConnected = false;
    String serverIP;

    Adafruit_SSD1306 display;

    void showOLEDMessage(const String &msg);
    void blinkLED(int times);
    void sendESPConnectedMessage();
    void checkAllConnectedNotification();
    void checkForServerBroadcast();
};

#endif
