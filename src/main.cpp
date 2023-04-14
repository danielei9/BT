#include <Arduino.h>
#include "base64.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <cstring>


static uint8_t DEVEUI_DEF[8]= { 0x17, 0x1f, 0xe2, 0x1d, 0x71, 0xB3, 0x03, 0x54 }  ; // 5403b3711de21f17 139971e1a31d4764
static uint8_t APPKEY_DEF[16] = {0x06, 0x26, 0x35, 0x77, 0xC3, 0xBB, 0xC9, 0xEB, 0x2F, 0xEF, 0x99, 0x13, 0x5E, 0xF0, 0xF6, 0xA5};
int TIME_TX = 30000; // ms 
uint8_t C1 = 21;  
uint8_t C12 = 23;  
uint8_t C21 = 26;
uint8_t C2 = 6; 
uint8_t MAX_VALUE = 50; 
#include "BluetoothSerial.h"



#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;

  String bytesToString(uint8_t*H, size_t length) {
    String result = "";
    for (int i = 0; i < length; i++) {
        if (H[i] < 16) {
            result += "0";
        }
        result += String(H[i], HEX);
    }
    return result;
    }


  char* base64_to_hex(const char* base64_str, char* hex_str) {
    unsigned char output[100];
    int base64_len = strlen(base64_str);
    int hex_index = 0;
    
    // Decode base64 string
    int binary_len = decode_base64((const unsigned char*)base64_str, output);
    
    // Convert each byte to hexadecimal
    for (int i = 0; i < binary_len; i++) {
        int high_nibble = output[i] >> 4;
        int low_nibble = output[i] & 0x0F;
        
        // Convert high nibble to hex
        if (high_nibble < 10) {
            hex_str[hex_index] = '0' + high_nibble;
        } else {
            hex_str[hex_index] = 'A' + (high_nibble - 10);
        }
        
        hex_index++;
        
        // Convert low nibble to hex
        if (low_nibble < 10) {
            hex_str[hex_index] = '0' + low_nibble;
        } else {
            hex_str[hex_index] = 'A' + (low_nibble - 10);
        }
        
       hex_index++;
    }
    
   hex_str[hex_index] = '\0';
    return hex_str;


}
void updateAppKey( std::string hex_str){
  std::vector<uint8_t> bytes;
    for (size_t i = 0; i < hex_str.length(); i += 2) {
        std::string byte_str = hex_str.substr(i, 2);
        uint8_t byte = std::stoi(byte_str, nullptr, 16);
        bytes.push_back(byte);
    }
   std::cout << "Bytes:";
    for (uint8_t byte : bytes) {
        std::cout << " " << std::hex << static_cast<int>(byte);
    }
    std::cout << std::endl;
 std::memcpy(&APPKEY_DEF[0], &bytes[0], sizeof(APPKEY_DEF));
   std::cout << "APPKEY_DEF = ";
      for (int i = 0; i < 16; i++) {
          std::cout << std::hex << static_cast<int>(APPKEY_DEF[i]) << " ";
      }
  std::cout << std::endl;

}

void updateDeveui( std::string hex_str){
  std::vector<uint8_t> bytes;
    for (size_t i = 0; i < hex_str.length(); i += 2) {
        std::string byte_str = hex_str.substr(i, 2);
        uint8_t byte = std::stoi(byte_str, nullptr, 16);
        bytes.push_back(byte);
    }
   std::cout << "Bytes:";
    for (uint8_t byte : bytes) {
        std::cout << " " << std::hex << static_cast<int>(byte);
    }
    std::cout << std::endl;
 std::memcpy(&DEVEUI_DEF[0], &bytes[0], sizeof(DEVEUI_DEF));
   std::cout << "DEVEUI_DEF = ";
      for (int i = 0; i < 8; i++) {
          std::cout << std::hex << static_cast<int>(DEVEUI_DEF[i]) << " ";
      }
  std::cout << std::endl;

}


void infoGeneric(){
    SerialBT.print("A0_");
    String appkey=bytesToString(APPKEY_DEF, sizeof(APPKEY_DEF));
    SerialBT.print(appkey);
    SerialBT.print("_");
    String deveui=bytesToString(DEVEUI_DEF, sizeof(DEVEUI_DEF));
    SerialBT.print(deveui);
    SerialBT.print("_");
    SerialBT.print(TIME_TX);
    Serial.println("A0_APPKEY_DEVEUI_TIME");
}  

void uptadeTimeTx(String arrivedMsg){
   if (arrivedMsg.startsWith("A3")) {
    String intNum = arrivedMsg.substring(2);
    if (intNum.toInt() != 0) {  // comprueba que el número es válido
      TIME_TX = intNum.toInt();
      infoGeneric();
    }
  }

}


void scheduleRXMessageUpdate(String arrivedMsg){
    const char* cs = arrivedMsg.c_str();
    char hex_str[100];
    char* messageHex = base64_to_hex(cs, hex_str);
    std::string str(messageHex);
    Serial.println(messageHex);
    if (str.find("A1") == 0) {
      updateAppKey(str.substr(2));
       infoGeneric();
    }
    if (str.find("A2") == 0) {
      updateDeveui(str.substr(2));
       infoGeneric();
    }
  
}



void setup(){
  Serial.begin(115200);
  SerialBT.begin("BUCHUISHERE"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");
}


void scheduleRXMessageGeneric(String s){
  if( s.startsWith("oAA=")){
   infoGeneric();
  }
}
  /*void scheduleRXMessageDevice(String s){
  if( s.startsWith("sAA=")){
    SerialBT.print("B0_");
    SerialBT.print(type);
    SerialBT.print("_");
    String c1=bytesToString(&C1, sizeof(C1));
    SerialBT.print(c1);
    SerialBT.print("_");
    String c2=bytesToString(&C2, sizeof(C2));
    SerialBT.print(c2);
    SerialBT.print("_");
    String c12=bytesToString(&C12, sizeof(C12));
    SerialBT.print(c12);
    SerialBT.print("_");
    String c21=bytesToString(&C21, sizeof(C21));
    SerialBT.print(c21);
    SerialBT.print("_");
    String max_value=bytesToString(&MAX_VALUE, sizeof(MAX_VALUE));
    SerialBT.print(max_value);
    Serial.println("B0_type_C1_C2_C12_C21_MAX");
  }
}*/

void enroute(String arrivedMsg){
  scheduleRXMessageGeneric(arrivedMsg);
  //scheduleRXMessageDevice(arrivedMsg);
  scheduleRXMessageUpdate(arrivedMsg);
  uptadeTimeTx(arrivedMsg);
}
void loop() {
  if (Serial.available()) { 
    SerialBT.write(Serial.read());
  }
  if (SerialBT.available()) {
    String arrivedMessage  = SerialBT.readString();
    Serial.println(arrivedMessage);
    enroute(arrivedMessage);
  }
  delay(20);
}
