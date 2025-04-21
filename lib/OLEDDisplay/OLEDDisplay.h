#ifndef OLED_DISPLAY_H
#define OLED_DISPLAY_H

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

class OLEDDisplay
{
public:
    OLEDDisplay(int width = 128, int height = 64, int sda = 5, int scl = 4);

    void begin();
    void showMessage(const String &msg, int textSize = 2);
    void animateConnecting();
    void clear();
    void showConnected();

private:
    int _width, _height, _sda, _scl;
    Adafruit_SSD1306 display;
    unsigned long lastUpdate = 0;
    int dotCount = 0;
};

#endif