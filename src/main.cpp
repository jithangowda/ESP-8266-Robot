#include <Arduino.h>
#include <ESPConnection.h>
#include <Servo.h>

#define CONTROL_PORT 4212

ESPConnection connection("ESP8266_Robot", "robot123");
WiFiUDP controlUdp;

Servo panServo;

void setup()
{
  connection.begin();
  controlUdp.begin(CONTROL_PORT);

  panServo.attach(13);
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

      int panIndex = msg.indexOf("PAN:");

      if (panIndex != -1)
      {
        // Extract PAN value
        String panVal = msg.substring(panIndex + 4);
        int pan = panVal.toInt();

        Serial.printf("Parsed ➜ PAN: %d\n", pan);

        int panMapped = map(pan, 0, 180, 40, 160);

        panServo.write(panMapped);
      }
    }
  }
}

void loop()
{
  connection.listenAndHandle();
  checkControlData();
}