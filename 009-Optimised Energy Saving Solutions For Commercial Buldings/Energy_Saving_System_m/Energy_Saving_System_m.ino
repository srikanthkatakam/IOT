
/*
 *  This sketch sends data via HTTP GET requests to api.thingspeak.com service.
 *
 *  You need to get API write Key at api.thingspeak.com  and paste them
 *  below. Or just customize this script to talk to other HTTP servers.
 *
 */

#include <ESP8266WiFi.h>
#include <DHT.h>
#define DHTPIN D2   //dht pin connected to D1
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

const char* ssid     = "your SSID";
const char* password = "password";

const char* host = "api.thingspeak.com";
const String channelsAPIKey = "thingspeak API";
const String talkBackAPIKey = "talkback API";
const String talkBackID = "talkback ID";
const unsigned int getTalkBackInterval = 10 * 1000;
const unsigned int updateChannelsInterval = 15 * 1000;
String talkBackCommand;

WiFiClient client;
void startWiFi();
void getTalkBack();
void updateChannels();
void Proximity_Mode();
void Sleep_Mode();
void Away_Mode();
void Eco_Mode();

long lastConnectionTimeChannels = 0;
boolean lastConnectedChannels = false;
int failedCounterChannels = 0;
long lastConnectionTimeTalkBack = 0;
boolean lastConnectedTalkBack = false;
int failedCounterTalkBack = 0;


int AC = D4;
int FAN = D3;
int LIGHT = D7;
int LIGHT1 = D6;
int PIR =D5;  //
int val =0;
int LDR = A0;
float Illumination;
int I;
float t;
float h;
String readString;

void setup()
{
  Serial.begin(115200);
  
  delay(10);
  pinMode(AC, OUTPUT);
  pinMode(FAN, OUTPUT);
  pinMode(LIGHT, OUTPUT);
  pinMode(LIGHT1, OUTPUT);
  pinMode(PIR, INPUT);
  
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
 startWiFi();
}


void loop() {
delay(10000);
  h = dht.readHumidity();
  t = dht.readTemperature();
  I = analogRead(LDR);
  Illumination = map(I,0,1023,1000,1);
  val = digitalRead(PIR);
//----------------------------------------------------
 
Serial.print("connecting to ");
Serial.println(host);

//client.stop();
delay(1000);
getTalkBack();
delay(5000);
updateChannels();  

if(talkBackCommand == "ECOMODE") Eco_Mode();
if(talkBackCommand == "AWAYMODE") Away_Mode();
if(talkBackCommand == "PROXIMITYMODE") Proximity_Mode();
if(talkBackCommand == "SLEEPMODE") Sleep_Mode();

if (t < 24 || h < 24){
  digitalWrite(AC, LOW);
 }
 if (val==LOW) {
  digitalWrite(LIGHT1, LOW);
  digitalWrite(FAN, LOW);
 }
 if(Illumination >600) {
  digitalWrite(LIGHT, LOW);
 }
}


//----------------Eco Mode------------------------------------------
void Eco_Mode(){
  
if (t >28 || h >26){
  digitalWrite(AC, HIGH);
 } 
 if (Illumination <500){
  digitalWrite(LIGHT, HIGH);
 }  
}

//--------------- Proximity Mode---------------------------------  
 void Proximity_Mode(){
  if (val==HIGH){
  digitalWrite(LIGHT1, HIGH);
  digitalWrite(FAN, HIGH);
 } 
 
 }
 //---------------- Sleep Mode-----------------------------------
  void Sleep_Mode(){
  digitalWrite(AC, HIGH);
  digitalWrite(LIGHT, LOW);
  }
  
  //------------------Away Mode--------------------------------- 
  void Away_Mode(){
  digitalWrite(AC, LOW);
  digitalWrite(LIGHT, LOW);
  digitalWrite(LIGHT1, LOW);
  digitalWrite(FAN, LOW);
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
client.println("GET /talkbacks/7649/commands/execute?api_key=OIKA2DLBAJ7X385E");
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
Serial.print(charIn);
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
  
void updateChannels()
{
String tsData;
tsData="field1="+String (h)+"&field2="+String (t)+"&field3="+String(Illumination, DEC)+"&field4="+String(val);

Serial.println(tsData);
if(!client.connected() && (millis() - lastConnectionTimeChannels > updateChannelsInterval))
{
if (client.connect("api.thingspeak.com", 80))
{
client.print("POST /update HTTP/1.1\n");
client.print("Host: api.thingspeak.com\n");
client.print("Connection: close\n");
client.print("X-THINGSPEAKAPIKEY: "+channelsAPIKey+"\n");
client.print("Content-Type: application/x-www-form-urlencoded\n");
client.print("Content-Length: ");
client.print(tsData.length());
client.print("\n\n");
client.print(tsData);
lastConnectionTimeChannels = millis();

if (client.connected())
{
Serial.println("****************************************");
Serial.println("Update channels");
Serial.println();
Serial.println("Connecting to ThingSpeak...");
Serial.println();
Serial.println("Server response ->");
Serial.println();
failedCounterChannels = 0;
while(client.connected() && !client.available()) delay(1); //waits for data
while (client.connected() || client.available())
{
char charIn = client.read();
Serial.print(charIn);
//client.stop();
}
Serial.println();
Serial.println();
Serial.println("...disconnected");
Serial.println();
}
else
{
failedCounterChannels++;
Serial.println("Connection to ThingSpeak failed ("+String(failedCounterChannels, DEC)+")");
Serial.println();
lastConnectionTimeChannels = millis();
}
}
else
{
failedCounterChannels++;
Serial.println("Connection to ThingSpeak Failed ("+String(failedCounterChannels, DEC)+")");
Serial.println();
lastConnectionTimeChannels = millis();
}
}
if (failedCounterChannels > 3 ) {startWiFi();}

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
  
  
