#include "ESPConnection.h"

ESPConnection::ESPConnection(const char *apName, const char *apPassword)
    : _apName(apName),
      _apPassword(apPassword),
      udp(),
      notifyUdp(),
      serverIP(),
      serverFound(false),
      allConnected(false),
      oled(128, 64, 5, 4) {}

void ESPConnection::begin()
{
    Serial.begin(115200);
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.println("Booting...");

    oled.begin();
    oled.showMessage("Connecting");

    WiFiManager wm;
    if (!wm.autoConnect(_apName, _apPassword))
    {
        Serial.println("WiFi failed.");
        ESP.restart();
    }

    Serial.print("Connected to: ");
    Serial.println(WiFi.SSID());
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());

    udp.begin(UDP_PORT);
    notifyUdp.begin(NOTIFY_PORT);

    Serial.println("Listening for server and notifications...");
}

void ESPConnection::blinkLED(int times)
{
    for (int i = 0; i < times; i++)
    {
        digitalWrite(LED_BUILTIN, HIGH);
        delay(300);
        digitalWrite(LED_BUILTIN, LOW);
        delay(300);
    }
}

void ESPConnection::sendESPConnectedMessage()
{
    static bool messageSent = false;
    if (serverFound && !messageSent)
    {
        WiFiUDP sender;
        sender.beginPacket(serverIP.c_str(), 4211);
        sender.write("ESP8266 Connected");
        sender.endPacket();
        messageSent = true;
        Serial.println("[ESP8266] Sent ESP8266 Connected message to server.");
    }
}

void ESPConnection::checkForServerBroadcast()
{
    char buf[255];
    int size = udp.parsePacket();
    if (size)
    {
        int len = udp.read(buf, sizeof(buf) - 1);
        if (len > 0)
        {
            buf[len] = '\0';
            String msg = String(buf);
            if (msg.startsWith("SERVER_IP:") && !serverFound)
            {
                serverIP = msg.substring(10);
                serverFound = true;
                Serial.print("✅ Server IP: ");
                Serial.println(serverIP);
                blinkLED(3);
                digitalWrite(LED_BUILTIN, HIGH);
                sendESPConnectedMessage();
            }
        }
    }
}

void ESPConnection::checkAllConnectedNotification()
{
    char buf[64];
    int size = notifyUdp.parsePacket();
    if (size)
    {
        int len = notifyUdp.read(buf, sizeof(buf) - 1);
        if (len > 0)
        {
            buf[len] = '\0';
            String msg = String(buf);
            Serial.print("[ESP8266] Got notify: ");
            Serial.println(msg);
            if (msg == "ALL_CONNECTED" && !allConnected)
            {
                allConnected = true;
                oled.showConnected();
                blinkLED(3);

                Serial.println("[ESP8266] ✅ All connected!");
            }
        }
    }
}

void ESPConnection::listenAndHandle()
{
    if (!serverFound)
    {
        checkForServerBroadcast();
        oled.animateConnecting();
    }
    else
    {
        sendESPConnectedMessage();
        checkAllConnectedNotification();
    }
}
