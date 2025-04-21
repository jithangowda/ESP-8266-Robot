#ifndef ESP_CONNECTION_H
#define ESP_CONNECTION_H

#include <ESP8266WiFi.h>
#include <WiFiManager.h>
#include <WiFiUdp.h>
#include <Arduino.h>
class ESPConnection
{
public:
    ESPConnection(const char *apName, const char *apPassword);
    void begin();
    void blinkLED(int times);
    void listenForServer();
    bool isServerFound();

private:
    const char *_apName;
    const char *_apPassword;
    WiFiUDP udp;
    const int UDP_PORT = 4210;
    bool serverFound;
    String serverIP;
    void sendESPConnectedMessage();
};

#endif