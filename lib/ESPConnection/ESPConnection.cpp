#include "ESPConnection.h"

ESPConnection::ESPConnection(const char *apName, const char *apPassword)
    : _apName(apName), _apPassword(apPassword), display(128, 64, &Wire, -1) {}

void ESPConnection::begin()
{
    Serial.begin(115200);
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.println("Booting...");

    Wire.begin(5, 4); // SDA=D1(GPIO5), SCL=D2(GPIO4)
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
    {
        Serial.println("OLED init failed");
    }
    else
    {
        display.clearDisplay();
        display.setTextSize(2);
        display.setTextColor(SSD1306_WHITE);
        showOLEDMessage("Connecting...");
    }

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

void ESPConnection::showOLEDMessage(const String &msg)
{
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println(msg);
    display.display();
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
                showOLEDMessage("Connected\nto robot");
                blinkLED(5);
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
    }
    else
    {
        sendESPConnectedMessage();
        checkAllConnectedNotification();
    }
}
