#include "OLEDDisplay.h"

OLEDDisplay::OLEDDisplay(int width, int height, int sda, int scl)
    : _width(width), _height(height), _sda(sda), _scl(scl), display(width, height, &Wire, 0x3C) {} // Initialize in order

void OLEDDisplay::begin()
{
    Wire.begin(_sda, _scl);

    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
    {
        Serial.println("❌ OLED init failed");
        return;
    }

    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    showMessage("Booting...");
}

void OLEDDisplay::showMessage(const String &msg, int textSize)
{
    display.clearDisplay();
    display.setTextSize(textSize);
    display.setCursor(0, 0);
    display.println(msg);
    display.display();
}

void OLEDDisplay::clear()
{
    display.clearDisplay();
    display.display();
}

void OLEDDisplay::animateConnecting()
{
    unsigned long currentMillis = millis();
    if (currentMillis - lastUpdate >= 300) // Faster animation speed
    {
        lastUpdate = currentMillis;
        dotCount = (dotCount + 1) % 4;

        String dots = "";
        for (int i = 0; i < dotCount; i++)
        {
            dots += ".";
        }

        showMessage("Connecting" + dots);
    }
}

void OLEDDisplay::showConnected()
{
    display.clearDisplay();

    // Line 1: center "Connected"
    int16_t x1, y1;
    uint16_t w, h;
    display.setTextSize(2);
    display.getTextBounds("Connected", 0, 0, &x1, &y1, &w, &h);
    display.setCursor((display.width() - w) / 2, 0);
    display.println("Connected");

    // Line 2: center "to"
    display.getTextBounds("to", 0, 0, &x1, &y1, &w, &h);
    display.setCursor((display.width() - w) / 2, 24);
    display.println("to");

    // Line 3: center "robot"
    display.getTextBounds("robot", 0, 0, &x1, &y1, &w, &h);
    display.setCursor((display.width() - w) / 2, 48);
    display.println("robot");

    display.display();
}