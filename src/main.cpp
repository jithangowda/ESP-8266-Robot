#include <Arduino.h>
#include <ESPConnection.h>
#include <Servo.h>

#define CONTROL_PORT 4212

ESPConnection connection("ESP8266_Robot", "robot123");
WiFiUDP controlUdp;

Servo tiltServo;
Servo panServo;

void setup()
{
  connection.begin();
  controlUdp.begin(CONTROL_PORT);

  tiltServo.attach(2); // Tilt servo on D2 (GPIO4)
  panServo.attach(13); // Pan servo on D7 (GPIO13)
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

      // TILT
      int tiltIndex = msg.indexOf("TILT:");
      if (tiltIndex != -1)
      {
        String tiltVal = msg.substring(tiltIndex + 5);
        int tilt = tiltVal.toInt();

        Serial.printf("Parsed ➜ TILT: %d\n", tilt);

        tiltServo.write(tilt);
      }

      // PAN
      int panIndex = msg.indexOf("PAN:");
      if (panIndex != -1)
      {
        String panDirection = msg.substring(panIndex + 4);
        Serial.printf("Parsed ➜ PAN: %s\n", panDirection.c_str());

        if (panDirection == "right")
        {
          panServo.write(40);
        }
        else if (panDirection == "left")
        {
          panServo.write(140);
        }
        else if (panDirection == "stop")
        {
          panServo.write(90);
        }
      }
    }
  }
}

void loop()
{
  connection.listenAndHandle();
  checkControlData();
}

// Servo 1 - pan ( horizontal) - D7 - gpio13  (360 deg servo)

// Servo 2 - tilt (vertical) - D4 - gpio2 ( mid 90, up 0)
