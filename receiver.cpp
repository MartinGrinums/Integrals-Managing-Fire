#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <RadioLib.h>
#include <WiFi.h>
#include <PubSubClient.h>

//#include "LoRa.h"
#define LoRa_MOSI 10
#define LoRa_MISO 11
#define LoRa_SCK 9

#define LoRa_nss 8
#define LoRa_dio1 14
#define LoRa_nrst 12
#define LoRa_busy 13

const char* ssid = "a21s";
const char* pass = "internets";

const char* mqqt_broker_addr = "192.168.60.102";

long lastMsg = 0;
long lastRcv = 0;

String received = "";

WiFiClient espClient;
PubSubClient client(espClient);
//dont use pin 27
SX1262 radio = new Module(LoRa_nss, LoRa_dio1, LoRa_nrst, LoRa_busy);

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  // Feel free to add more if statements to control more GPIOs with MQTT

  // If a message is received on the topic esp32/output, you check if the message is either "on" or "off". 
  // Changes the output state according to the message
  
}

void setup(){
 // lora::setup;
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
  //setup_wifi();
  //client.setServer(mqqt_broker_addr, 1883);
  //client.setCallback(callback);
}
void receive(){
   int state = radio.receive(received);
   if(state == RADIOLIB_ERR_NONE){
     Serial.println(received);
   }
   else{
     Serial.println(state);
   }
}





void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("Node")) {
      Serial.println("connected");
      // Subscribe
      client.subscribe("node");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void loop() {
  /*if (!client.connected()) {
    reconnect();
  }
  client.loop();

  
  if (now - lastMsg > 5000) {
    lastMsg = now;
    int str_len = received.length() + 1;
    char message[str_len];
    received.toCharArray(message, str_len);
    client.publish("node", message);
  }*/

  long now = millis();
  if(now - lastRcv > 1000) {
    receive();
    lastRcv = now;
  }
}
