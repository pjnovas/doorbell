#include "config.h"

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

String eventName = EVENT_NAME;
String key = KEY;

void connectToWifi()
{
  int tries = 20;

  WiFi.mode(WIFI_STA);
  WiFi.hostname(WIFI_HOSTNAME);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    blink(1, 200, 200);
    tries--;

    if (tries < 0 || WiFi.status() == WL_CONNECT_FAILED)
    {
      blink(2, 200, 1000);
      ESP.restart();
      return;
    }
  }

  blinkOK();
}

void sendNotification()
{
#ifdef LED_BUILTIN
  digitalWrite(LED_BUILTIN, LOW);
#endif

  WiFiClient client;
  HTTPClient http;

  if (http.begin(client, "http://maker.ifttt.com/trigger/" + eventName + "/with/key/" + key))
  {
    int httpCode = http.GET();
    if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY)
    {
      blinkOK();
      http.end();
      return;
    }
  }

  http.end();
  blinkFail();
}

void setup()
{
#ifdef LED_BUILTIN
  pinMode(LED_BUILTIN, OUTPUT);
#endif

  delay(500);

  connectToWifi();
  sendNotification();

  // Go sleep
  ESP.deepSleep(0);
}

void blink(int times, int offTime, int onTime)
{
#ifdef LED_BUILTIN
  for (int i = 0; i <= times; i++)
  {
    digitalWrite(LED_BUILTIN, LOW);
    delay(onTime);

    digitalWrite(LED_BUILTIN, HIGH);
    delay(offTime);
  }
#endif
}

void blinkOK() // 3 quick blinks
{
  blink(3, 300, 300);
}

void blinkFail()
{
  // will block on led flashes ONLY IF DEBUG IS ACTIVE
  // otherwise it will go on to deep sleep till next reset

#ifdef LED_BUILTIN
  while (true)
  {
    blink(1, 1000, 1000);
  }
#endif
}

void loop() {} // NOT USED
