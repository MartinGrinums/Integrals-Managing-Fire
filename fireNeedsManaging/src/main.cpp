#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <iostream>
#include <EEPROM.h>
#include <RadioLib.h>
#include <AHT10.h>
#include <u8g2lib.h>
#define LoRa_MOSI 10
#define LoRa_MISO 11
#define LoRa_SCK 9
#define LoRa_nss 8
#define LoRa_dio1 14
#define LoRa_nrst 12
#define LoRa_busy 13
#define oled_scl 18
#define oled_sda 17
#define oled_rst 21
U8G2_SSD1306_128X64_NONAME_1_SW_I2C u8g2(U8G2_R0, oled_scl, oled_sda, oled_rst);
AHT10 myAHT10(AHT10_ADDRESS_0X38);
SX1262 radio = new Module(LoRa_nss, LoRa_dio1, LoRa_nrst, LoRa_busy);
void setup(){
  //lora::setup;
  Serial.begin(115200);
  SPI.begin(LoRa_SCK, LoRa_MISO, LoRa_MOSI, LoRa_nss);

  // initialize SX1262 with default settings
  Serial.print(F("[SX1262] Initializing ... "));
  int state = radio.begin();
  if (state == RADIOLIB_ERR_NONE)
  {
    Serial.println(F("success!"));
  }
  else
  {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true);
  }
  while (myAHT10.begin() != true)
  {
    Serial.println(F("AHT10 not connected or fail to load calibration coefficient")); //(F()) save string to flash & keeps dynamic memory free
    delay(5000);
  }
  Serial.println(F("AHT10 OK"));
  if (!u8g2.begin()){
    Serial.println("OCD failed");
  }else{
    Serial.println("OCD successful");
  }
}
void transmitter (String outgoing){
Serial.print(F("[SX1262] Transmitting packet ... "));

  int state = radio.transmit(outgoing);

  if (state == RADIOLIB_ERR_NONE)
  {
    // the packet was successfully transmitted
    Serial.println(F("success!"));
  }
  else if (state == RADIOLIB_ERR_PACKET_TOO_LONG)
  {
    // the supplied packet was longer than 256 bytes
    Serial.println(F("too long!"));
  }
  else if (state == RADIOLIB_ERR_TX_TIMEOUT)
  {
    // timeout occured while transmitting packet
    Serial.println(F("timeout!"));
  }
  else
  {
    // some other error occurred
    Serial.print(F("failed, code "));
    Serial.println(state);
  }
}

void loop() {
 double humidity = myAHT10.readHumidity();
 double temperature = myAHT10.readTemperature();
 String data = String(humidity) + "," + String(temperature);
 Serial.println(data);
  if(humidity >= 65){
 transmitter(data);
 Serial.println(data);
   u8g2.firstPage();
  do
  {
    u8g2.setFont(u8g2_font_ncenB10_tr);
    u8g2.drawStr(0, 24, "FIRE! FIRE!");
  } while (u8g2.nextPage());
 }else if (temperature >= 100){
     u8g2.firstPage();
  do
  {
    u8g2.setFont(u8g2_font_ncenB10_tr);
    u8g2.drawStr(0, 24, "FIRE! FIRE!");
  } while (u8g2.nextPage());
 transmitter(data);
 Serial.println(data);
 }else{
  u8g2.clear();
 }
}
