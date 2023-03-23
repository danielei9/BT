#include <Arduino.h>
//This example code is in the Public Domain (or CC0 licensed, at your option.)
//By Evandro Copercini - 2018
//
//This example creates a bridge between Serial and Classical Bluetooth (SPP)
//and also demonstrate that SerialBT have the same functionalities of a normal Serial
static uint8_t DEVEUI_DEF[8]= { 0x17, 0x1f, 0xe2, 0x1d, 0x71, 0xB3, 0x03, 0x54 }  ; // 5403b3711de21f17 139971e1a31d4764
static uint8_t APPKEY_DEF[16] = {0x06, 0x26, 0x35, 0x77, 0xC3, 0xBB, 0xC9, 0xEB, 0x2F, 0xEF, 0x99, 0x13, 0x5E, 0xF0, 0xF6, 0xA5};
int TIME_TX = 30000; // ms 

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
    SerialBT.print("A0_");
    //TODO: Convertir a string como : 06263577C3BBC9EB2FEF99135EF0F6A5
    // SerialBT.print(APPKEY_DEF);
    SerialBT.print("_");
    //TODO: Convertir a string como : 171fe21d71B30354
    // SerialBT.print(DEVEUI_DEF);
    SerialBT.print("_");
    //TODO: Convertir a string como : 30000
    SerialBT.print(TIME_TX);
    SerialBT.print("_");
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
