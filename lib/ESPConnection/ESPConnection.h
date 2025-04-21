#ifndef ESP_CONNECTION_H
#define ESP_CONNECTION_H

#include <Arduino.h>
#include <WiFiManager.h>
#include <WiFiUdp.h>
#include <OLEDDisplay.h>


#define UDP_PORT 4210
#define NOTIFY_PORT 4213

class ESPConnection
{
public:
    ESPConnection(const char *apName, const char *apPassword);
    void begin();
    void listenAndHandle();

private:
    const char *_apName;
    const char *_apPassword;
    WiFiUDP udp;
    WiFiUDP notifyUdp;

    String serverIP;
    bool serverFound = false;
    bool allConnected = false;

    OLEDDisplay oled;

    void blinkLED(int times);
    void sendESPConnectedMessage();
    void checkForServerBroadcast();
    void checkAllConnectedNotification();
};

#endif
