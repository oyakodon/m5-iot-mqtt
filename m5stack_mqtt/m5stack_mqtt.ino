#include <M5Stack.h>

#include <WiFi.h>
#include <PubSubClient.h>

const char *ssid = "<SSID_HERE>";
const char *password = "<PASSPHRASE_HERE>";
const char *mqtt_server = "<MQTT_BROKER_ADDRESS_HERE>";

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE (50)
char msg[MSG_BUFFER_SIZE];
int value = 0;

void connectMqtt()
{
  while (!mqttClient.connected())
  {
    String clientId = "M5Stack-";
    clientId += String(random(0xffff), HEX);
    if (mqttClient.connect(clientId.c_str()))
    {
      M5.Lcd.println("MQTT Connected.");
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" try again in 5 seconds");

      delay(5000);
    }
  }
}

void setup()
{
  M5.begin(true, false, true, false);
  M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.setTextSize(2);

  M5.Lcd.println("WiFi Connecting...");

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
  }

  M5.Lcd.println("WiFi Connected.");
  M5.Lcd.println("---");

  mqttClient.setServer(mqtt_server, 1883);
}

void loop()
{
  M5.update();

  if (!mqttClient.connected())
  {
    connectMqtt();
  }

  // unsigned long now = millis();
  // if (now - lastMsg > 2000)
  // {
  //   lastMsg = now;

  //   ++value;
  //   snprintf(msg, MSG_BUFFER_SIZE, "hello world #%ld", value);

  //   client.publish("M5Stack", msg);
  // }

  if (M5.BtnA.wasPressed())
  {
    String msg;
    msg += "{\"timestamp\": " + String(millis()) + ",";
    msg += "\"button\": \"A\"}";

    mqttClient.publish("M5Stack", msg.c_str());
  }
  if (M5.BtnB.wasPressed())
  {
    String msg;
    msg += "{\"timestamp\": " + String(millis()) + ",";
    msg += "\"button\": \"B\"}";

    mqttClient.publish("M5Stack", msg.c_str());
  }
  if (M5.BtnC.wasPressed())
  {
    String msg;
    msg += "{\"timestamp\": " + String(millis()) + ",";
    msg += "\"button\": \"C\"}";

    mqttClient.publish("M5Stack", msg.c_str());
  }
}
