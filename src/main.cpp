#include <Arduino.h>
#include <ESPConnection.h>
#include <Servo.h>

#define CONTROL_PORT 4212

ESPConnection connection("ESP8266_Robot", "robot123");
WiFiUDP controlUdp;

Servo tiltServo;

void setup()
{
  connection.begin();
  controlUdp.begin(CONTROL_PORT);

  tiltServo.attach(2);
  Serial.println("Setup complete");
}

void checkControlData()
{
  char buf[64];
  int size = controlUdp.parsePacket();
  if (size)
  {
    int len = controlUdp.read(buf, sizeof(buf) - 1);
    if (len > 0)
    {
      buf[len] = '\0';
      String msg = String(buf);
      Serial.print("[ESP8266] Control data: ");
      Serial.println(msg);

      int tiltIndex = msg.indexOf("TILT:");

      if (tiltIndex != -1)
      {
        String tiltVal = msg.substring(tiltIndex + 5);
        int tilt = tiltVal.toInt();

        Serial.printf("Parsed ➜ TILT: %d\n", tilt);

        tiltServo.write(tilt);
      }
    }
  }
}

void loop()
{
  connection.listenAndHandle();
  checkControlData();
}