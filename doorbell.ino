#include "config.h"

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char *ssid = WIFI_SSID;
const char *password = WIFI_PASSWORD;
const char *clientId = WIFI_HOSTNAME;

const char *mqtt_server = MQTT_HOST;
const unsigned int mqtt_port = MQTT_PORT;

const char *mqtt_user = MQTT_USER;
const char *mqtt_pass = MQTT_PASS;
const char *mqtt_topic = MQTT_TOPIC;

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi()
{
  WiFi.mode(WIFI_STA);
  WiFi.hostname(WIFI_HOSTNAME);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
  }
}

void reconnect()
{
  if (client.connect(clientId, mqtt_user, mqtt_pass))
  {
    client.publish(mqtt_topic, "RING");
    delay(5000);
    ESP.deepSleep(0);
  }

  delay(5000);
}

void setup()
{
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
}

void loop()
{
  if (!client.connected())
  {
    reconnect();
  }
  client.loop();
}
