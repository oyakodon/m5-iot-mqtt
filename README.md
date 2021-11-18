# m5-iot-mqtt

IoT sensor hub using MQTT, BLE for M5 series

## Environment

- Arduino v1.8.16
- Libraries
  - M5Stack (v0.3.9) by M5Stack
  - M5StickC (v0.2.5) by M5Stack
  - M5Atom (v0.0.7) by M5Stack,Hades
  - PubSubClient (v2.8.0) by Nick O'Leary

## Contents

|Folder|Content|
|:-|:-|
|m5atom_ble_peripheral|M5Atom (Lite) as BLE Peripheral, Button event|
|m5stack_ble_central|M5Stack as BLE Central|
|m5stack_mqtt|M5Stack MQTT (publish in response to 3 buttons)|
|m5stack_sensor_hub|M5Stack as BLE Central, MQTT hub|
|m5stickc_mqtt|M5StickC MQTT (publish in response to a button)|
