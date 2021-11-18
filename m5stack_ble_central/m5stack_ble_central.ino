/*
- M5Stackの設定から、「Partition scheme」を「No OTA (Large APP)」にする！
  - WiFiとBLEを同時に利用するとプログラムサイズが1.28MBを超過してしまう。
  - No OTA: 2MB
*/
#include <M5Stack.h>

#include <WiFi.h>
#include "BLEDevice.h"

const char *ssid = "<SSID_HERE>";
const char *password = "<PASSPHRASE_HERE>";

const char *ntpServer = "ntp.nict.jp";

// generated from uuid generator <https://www.uuidgenerator.net/version4>
// M5Atom側と同じ値を指定する
static BLEUUID SERVICE_UUID("a949570d-a843-413b-8b22-b8e2a586fc3b");
static BLEUUID CHARACTERISTIC_UUID("97a727b1-12d4-46b0-8b43-c0f0f6d64419");
BLERemoteCharacteristic *pRemoteCharacteristic;
BLEAdvertisedDevice *myDevice;

bool doConnect = false;
bool connected = false;

void log(const String &msg)
{
  static int y = 0;
  y++;
  if (y > 13)
  {
    M5.Lcd.fillScreen(TFT_BLACK);
    M5.Lcd.setCursor(0, 0);

    y = 0;
  }

  M5.Lcd.println(msg);
}

void notifyCallback(BLERemoteCharacteristic *pBLERemoteCharacteristic, uint8_t *pData, size_t length, bool isNotify)
{
  log("Received: " + String(pData[0]));
}

class MyClientCallback : public BLEClientCallbacks
{
  void onConnect(BLEClient *pclient)
  {
    log("BLE device connected.");
  }

  void onDisconnect(BLEClient *pclient)
  {
    connected = false;
  }
};

bool connectToServer()
{
  BLEClient *pClient = BLEDevice::createClient();
  pClient->setClientCallbacks(new MyClientCallback());

  pClient->connect(myDevice);

  BLERemoteService *pRemoteService = pClient->getService(SERVICE_UUID);
  if (pRemoteService == nullptr)
  {
    pClient->disconnect();
    return false;
  }
  pRemoteCharacteristic = pRemoteService->getCharacteristic(CHARACTERISTIC_UUID);
  if (pRemoteCharacteristic == nullptr)
  {
    pClient->disconnect();
    return false;
  }

  if (pRemoteCharacteristic->canNotify())
  {
    pRemoteCharacteristic->registerForNotify(notifyCallback);
  }

  connected = true;
  doConnect = false;
}

class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks
{
  void onResult(BLEAdvertisedDevice advertisedDevice)
  {
    if (advertisedDevice.haveServiceUUID() && advertisedDevice.isAdvertisingService(SERVICE_UUID))
    {
      BLEDevice::getScan()->stop();
      myDevice = new BLEAdvertisedDevice(advertisedDevice);
      doConnect = true;
    }
  }
};

void setup()
{
  M5.begin(true, false, true, false);
  M5.Speaker.mute();
  M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.setTextSize(2);

  log("WiFi connecting...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
  }
  log("connected!");
  log("---");

  configTime(9 * 3600 /*JST*/, 0, ntpServer);
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo))
  {
    log("ERROR - NTP");
  }
  else
  {
    log("T: " + String(mktime(&timeinfo)));
  }
  log("---");

  BLEDevice::init("M5Stack");

  BLEScan *pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true);
}

void loop()
{
  M5.update();

  if (doConnect)
  {
    connectToServer();
  }

  if (!connected)
  {
    BLEDevice::getScan()->start(0, true);
  }

  delay(1000);
}
