#include <M5Atom.h>

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

// generated from uuid generator <https://www.uuidgenerator.net/version4>
#define SERVICE_UUID "a949570d-a843-413b-8b22-b8e2a586fc3b"
#define CHARACTERISTIC_UUID "97a727b1-12d4-46b0-8b43-c0f0f6d64419"
BLECharacteristic *pCharacteristic;

void sendMessage(int v)
{
  pCharacteristic->setValue(v);
  pCharacteristic->notify();
}

void setup()
{
  M5.begin(true, true, true);
  M5.dis.clear();

  BLEDevice::init("M5Atom-Lite");
  BLEServer *pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(SERVICE_UUID);
  pCharacteristic = pService->createCharacteristic(
      CHARACTERISTIC_UUID,
      BLECharacteristic::PROPERTY_NOTIFY);

  pService->start();
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06); // iPhoneからの接続のために必要らしい
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
}

float t;
void loop()
{
  M5.update();

  t = (millis() % 2000) / 1000.0f;
  M5.dis.drawpix(0, (CRGB)((uint8_t)(constrain(t <= 1 ? t : 2 - t, 0.0, 1.0) * 0xFF) << 8));

  if (M5.Btn.wasReleasefor(100))
  {
    sendMessage(1);
  }
}
