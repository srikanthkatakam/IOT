//Cold Storage Monitoring and Control

#include <ESP8266WiFi.h>
#include "DHT.h"
#define DHTPIN D2     // what pin we're connected to
#define DHTTYPE DHT11   // define type of sensor DHT 11
DHT dht (DHTPIN, DHTTYPE);

const char* ssid     = "your SSID";
const char* password = "password";

const char* host = "api.thingspeak.com";
const char* host1 = "api.pushingbox.com";

const char* privateKey = "thingspeak write API";
const String talkBackAPIKey = "talkback API";
const String talkBackID = "talkback ID";

const unsigned int getTalkBackInterval = 10 * 1000;

String talkBackCommand;
//String clientResponce;
WiFiClient client;

String PushingBoxNotifier = "/pushingbox?devid=vB45E50FE74B355A";
//String PushingBoxNotifier1 = "GET /pushingbox?devid=v0F25CDA0138DEDF";

long lastConnectionTimeChannels = 0;
boolean lastConnectedChannels = false;
int failedCounterChannels = 0;
long lastConnectionTimeTalkBack = 0;
boolean lastConnectedTalkBack = false;
int failedCounterTalkBack = 0;

void setup() {

 Serial.begin(115200);
  
  delay(10);

  // We start by connecting to a WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  pinMode(D3, OUTPUT); // Buzzer
  pinMode(D4, OUTPUT); //AC Condition

  dht.begin();
  startWiFi();
}

void loop() {

  delay(15000);
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  int Flame = analogRead(A0);
  
  Serial.println(Flame);
    
  if(Flame < 80)
{
  //Serial.println("detected the falme");
  digitalWrite(D3, HIGH);
  delay(5000); 
  TriggerPushingbox();
  delay(2000);
  }
else{
//Serial.println("flame is not detected");
digitalWrite(D3, LOW);
delay(1000);
}

 
getTalkBack();
delay(1000);

if(talkBackCommand == "ACON") digitalWrite(D4, HIGH);
if(talkBackCommand == "ACOFF")digitalWrite(D4, LOW);

  Serial.print("connecting to ");
  Serial.println(host);
  
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
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
  url += t;
  url += "&field2=";
  url += h;
  url += "&field3=";
  url += Flame;
 

  
  Serial.print("Requesting URL: ");
  Serial.println(url);
  
  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
 
   delay(10);
  
  while(client.connected() && !client.available()) delay(1); //waits for data
  while (client.connected() || client.available())
  {
  char charIn = client.read();
  //Serial.print(charIn);
  }
  Serial.println();
  Serial.println("closing connection");
  client.stop();
  }

void TriggerPushingbox(){
  
  Serial.print("connecting to ");
  Serial.println(host1);
  const int httpPort = 80;
  
  if (client.connect(host1, httpPort)) {
  Serial.println("Client Connected to PushingBox");
  client.print(String("GET ") + PushingBoxNotifier + " HTTP/1.1\r\n" +
               "Host: " + host1 + "\r\n" + 
               "Connection: close\r\n\r\n");
  
  //client.println("GET /pushingbox?devid=vB45E50FE74B355A");
  //client.println(PushingBoxNotifier1);
  }
  
  if (client.connected()){
  while(client.connected() && !client.available()) delay(10); //waits for data
  while (client.connected() || client.available()){
  char charIn = client.read();
  Serial.print(charIn);
  //client.stop();
  }
  }
  client.stop();
  Serial.flush();
}

void getTalkBack()
{
String tsData="";
tsData=talkBackID + "/commands/execute?api_key=" + talkBackAPIKey;

if((!client.connected() && (millis() - lastConnectionTimeTalkBack > getTalkBackInterval)))
{
if (client.connect("api.thingspeak.com", 80))
{

//client.println("GET /talkbacks/"+tsData+" HTTP/1.0");
client.println("GET /talkbacks/13385/commands/execute?api_key=TE3BERCIET03K5EV");
//client.println();

Serial.println("GET /talkbacks/"+tsData+" HTTP/1.0");
lastConnectionTimeTalkBack = millis();

if (client.connected())
{
Serial.println(".........................");
Serial.println("GET TalkBack command");
Serial.println();
Serial.println("Connecting to ThingsApp...");
Serial.println();
Serial.println();
Serial.println("Server response ->");
Serial.println();
failedCounterTalkBack = 0;
talkBackCommand="";
while(client.connected() && !client.available()) delay(10); //waits for data
while (client.connected() || client.available())
{
char charIn = client.read();
Serial.print(charIn);https://api.thingspeak.com/update?api_key=9RB1XEZ5VE0H75AE&field1=0
talkBackCommand += charIn;
}
//talkBackCommand=talkBackCommand.substring(talkBackCommand.indexOf("open")+5);
//Serial.println();
//Serial.println();
//Serial.println("...disconnected");
Serial.println();
Serial.println("-----------------------");
Serial.print("Command -> ");
Serial.println(talkBackCommand);
Serial.println("-----------------------");
Serial.println();
}
else
{
failedCounterTalkBack++;
Serial.println("Connection to ThingSpeak failed ("+String(failedCounterTalkBack, DEC)+")");
Serial.println();
lastConnectionTimeChannels = millis();
}
}
else
{
failedCounterTalkBack++;
Serial.println("Connection to ThingSpeak Failed ("+String(failedCounterTalkBack, DEC)+")");
Serial.println();
lastConnectionTimeTalkBack = millis();
}
}
if (failedCounterTalkBack > 3 ) {startWiFi();}
client.stop();
Serial.flush();
}

void startWiFi()
{
client.stop();
Serial.println();
Serial.println("Connecting Arduino to network...");
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
  
   
