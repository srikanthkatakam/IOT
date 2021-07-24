#include <ESP8266WiFi.h>
#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>
#define led1 D1
#define led2 D3


RF24 radio(D2, D8);               // nRF24L01 (CE,CSN)
RF24Network network(radio);      // Include the radio in the network
const uint16_t this_node = 00;   // Address of this node in Octal format ( 04,031, etc)
const uint16_t node01 = 01;      // Address of the other node in Octal format
const uint16_t node02 = 02;      // Address of the other node in Octal format

unsigned long incomingData, tbcommand1;
int ldrvalue, irvalue;
int gatewaystatus,pole1status;
String tbcommand,tbrequest;

const unsigned long interval = 10;  //ms  // How often to send data to the other unit
unsigned long last_sent;            // When did we last send?

const char* ssid     = "SSID";//Enter the ssid of your router
const char* password = "password";//Enter the password of your router

const char* host = "api.thingspeak.com";
const char* privateKey = "thingspeak write key"; //place your write api key
//const String talkBackAPIKey = "0M6UCZWFK1XNEGTY";
//const String talkBackID = "30723";
const String talkBackAPIKey ="talkback API";
const String talkBackID = "talkback ID";

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
  connectwifi();
  
}
void loop() {
Serial.print("connecting to ");
Serial.println(host);

 Serial.println("please select auto or manual");//select 5 when you want control mode 
 delay(1000);
 getTalkBack();
 

if(tbcommand=="5")
{ Serial.println("manual mode selected");
  ManualMode(); 
  
} 
else
{
  AutomaticMode();//always the program runs in automatic mode unless the tbcommand is 5
}
}  
void AutomaticMode(){
  Serial.println("automatic mode started");
  ldrvalue = analogRead(A0);
  Serial.println(ldrvalue);
  irvalue = digitalRead(D0);
  Serial.println(irvalue);
  if (ldrvalue >= 500 ){
    analogWrite(led1, 10);
    analogWrite(led2, 10);
    gatewaystatus = 1;  
  if (ldrvalue >= 500 && irvalue == 1){
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
    gatewaystatus = 1;
  }
  }
    else if (irvalue == 0 || ldrvalue < 500) {
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);
    gatewaystatus = 0;
  }
    network.update();
  //===== Receiving =====//
  while ( network.available() ) {     // Is there any incoming data?
    RF24NetworkHeader header;
    network.read(header, &incomingData, sizeof(incomingData)); // Read the incoming data
   Serial.println(incomingData);
  }
  
  if (incomingData == 0){
    pole1status=0;
    Serial.print("Pole1 OFF :");
    Serial.println(incomingData);
   // delay(1000);
  }
  else if (incomingData == 1){
    pole1status=1;
    Serial.print("Pole1 ON :");
    Serial.println(incomingData);
    //delay(1000);
  }
  
  Upload_to_Cloud();
  
 }
//when tbcommand is 5 program enters mamual mode and we can select any operation mode either 100,50 or 1
//if we want to test it again select tbcommand as 5 and select manual mode operation 100,50 or 1 again.
 void ManualMode(){
  Serial.println("manual selected");
  
  Serial.println("please select manual mode opperation");// select manual mode operation in 10 sec
  delay(10000);
  getTalkBack();
  
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
    //RF24NetworkHeader header2(node02);   // (Address where the data is going)
    //bool ok1 = network.write(header2, &tbcommand1, sizeof(tbcommand1)); // Send the data
    
  }
  delay(10000);
 
 
  
  
}

  
  
  
 

 /*.....................................Uploading to Cloud...............................*/

void Upload_to_Cloud()
{
  Serial.print("connecting to ");
  Serial.println(host);
  
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  
  // We now create a URI for the request
  String url = "/update";
  url += "?api_key=";
  url += privateKey;
  url += "&field1=";
  url += gatewaystatus;
  url += "&field2=";
  url += pole1status;
  //url += "&field3=";
  //url += pole2status;
  
  
  Serial.print("Requesting URL: ");
  Serial.println(url);
  
  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  //delay(1000);
  
  // Read all the lines of the reply from server and print them to Serial
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
  
  Serial.println();
  Serial.println("closing connection");
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
//delay(100);
}
void connectwifi(){
   // We start by connecting to a WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
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
}
