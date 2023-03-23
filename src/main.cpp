#include <Arduino.h>
//This example code is in the Public Domain (or CC0 licensed, at your option.)
//By Evandro Copercini - 2018
//
//This example creates a bridge between Serial and Classical Bluetooth (SPP)
//and also demonstrate that SerialBT have the same functionalities of a normal Serial


#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;

void setup() {
  Serial.begin(115200);
  SerialBT.begin("BUCHUISHERE"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");
}
// IBOX STA

void scheduleRXMessage(String s){
  if( s.startsWith("sAA=")){
    SerialBT.println("B0_APPKEY_DEVEUI_TIME");
    Serial.println("B0_APPKEY_DEVEUI_TIME");
  }

}
void loop() {
  if (Serial.available()) { 
    SerialBT.write(Serial.read());
  }
  if (SerialBT.available()) {
    String arrivedMessage  = SerialBT.readString();
    Serial.println(arrivedMessage);
     scheduleRXMessage(arrivedMessage);
    
  }
  delay(20);
}
