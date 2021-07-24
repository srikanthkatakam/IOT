#include <ESP8266WiFi.h>
#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>
#define led1 D1
#define led2 10

RF24 radio(D2, D8);               // nRF24L01 (CE,CSN)
RF24Network network(radio);      // Include the radio in the network
const uint16_t this_node = 00;   // Address of this node in Octal format ( 04,031, etc)
const uint16_t node01 = 01;      // Address of the other node in Octal format
const uint16_t node02 = 02;

const char* ssid = "ssid";
const char* password = "password";

const char* host = "api.thingspeak.com";
const String talkBackAPIKey = "talkback API";
const String talkBackID = "talkback ID";

const unsigned long interval = 10;  //ms  // How often to send data to the other unit
unsigned long last_sent;            // When did we last send?

String tbcommand,tbrequest;
unsigned long tbcommand1;
WiFiClient client;

void setup() {
  Serial.begin(115200);
  SPI.begin();
  radio.begin();
  network.begin(90, this_node);  //(channel, node address)
  radio.setDataRate(RF24_2MBPS);
  pinMode(led1,OUTPUT);
  pinMode(led2,OUTPUT);
  analogWrite(led1,0);
  analogWrite(led2,0);
  startWiFi();
}
void loop() {
  
Serial.print("connecting to ");
Serial.println(host);
getTalkBack(); 
 delay(1000);

if(tbcommand=="1")
{
  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
 
}  
if(tbcommand=="50"){
  analogWrite(led1, 10);
  analogWrite(led2, 10);
 
  }
if(tbcommand=="100"){
  digitalWrite(led1, HIGH);
  digitalWrite(led2, HIGH);
  
  }
  tbcommand1 = tbcommand.toInt();
  network.update();
  //===== Sending =====//
  unsigned long now = millis();
  if (now - last_sent >= interval) {   // If it's time to send a data, send it!
    last_sent = now;
   
    RF24NetworkHeader header1(node01);   // (Address where the data is going)
    bool ok = network.write(header1, &tbcommand1, sizeof(tbcommand1)); // Send the data
    Serial.println(ok);
    RF24NetworkHeader header2(node02);   // (Address where the data is going)
    bool ok1 = network.write(header2, &tbcommand1, sizeof(tbcommand1)); // Send the data
    
  }
  delay(2000);
}

void getTalkBack()
{
  tbrequest="GET /talkbacks/"+ talkBackID + "/commands/execute?api_key=" + talkBackAPIKey;
  Serial.println(tbrequest);
  if(!client.connected())
  {
    if (client.connect("api.thingspeak.com", 80))
    {
    client.println(tbrequest);
      if (client.connected())
      {
        tbcommand="";
        while(client.connected() && !client.available()) delay(10); //waits for data
        while (client.connected() || client.available())
            {
              tbcommand = client.readStringUntil('\r');
            }
        Serial.print("Command -> ");
        Serial.println(tbcommand);
        Serial.println();
      }
  client.stop();
  Serial.flush();
 }
}
delay(100);
}

void startWiFi()
{
client.stop();
Serial.println();
Serial.print("Connecting NodeMCU to network...:");
Serial.println(ssid); 
WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  delay(1000);
  }
