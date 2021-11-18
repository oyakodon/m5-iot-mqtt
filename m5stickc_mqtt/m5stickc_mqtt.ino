#include <M5StickC.h>

#include <WiFi.h>
#include <PubSubClient.h>

const char *ssid = "<SSID_HERE>";
const char *password = "<PASSPHRASE_HERE>";
const char *mqtt_server = "<MQTT_BROKER_ADDRESS_HERE>";

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE (50)
char msg[MSG_BUFFER_SIZE];
int value = 0;

void setup()
{
  M5.begin(true, true, true);
  pinMode(GPIO_NUM_10, OUTPUT); // 内蔵LED (LOWで点灯)
  digitalWrite(GPIO_NUM_10, HIGH);
  M5.Lcd.setRotation(3); // 横画面
  M5.Lcd.fillScreen(TFT_BLACK);

  M5.Lcd.println("WiFi Connecting...");

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
  }

  M5.Lcd.println("WiFi Connected.");
  M5.Lcd.println("---");

  client.setServer(mqtt_server, 1883);
  // client.setCallback(callback);
}

void loop()
{
  while (!client.connected())
  {
    String clientId = "M5StickC-";
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str()))
    {
      M5.Lcd.println("MQTT Connected.");
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");

      delay(5000);
    }
  }

  // client.loop();

  unsigned long now = millis();
  if (now - lastMsg > 2000)
  {
    lastMsg = now;

    ++value;
    snprintf(msg, MSG_BUFFER_SIZE, "hello world #%ld", value);

    digitalWrite(GPIO_NUM_10, LOW);
    client.publish("M5StickC", msg);
    delay(100);
    digitalWrite(GPIO_NUM_10, HIGH);
  }
}
