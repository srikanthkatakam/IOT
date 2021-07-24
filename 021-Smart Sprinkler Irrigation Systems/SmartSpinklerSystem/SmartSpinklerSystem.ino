/*
 *  This sketch sends data via HTTP GET requests to api.thingspeak.com service.
 *
 *  You need to get API write Key at api.thingspeak.com  and paste them
 *  below. Or just customize this script to talk to other HTTP servers.
 *
 */

#include <ESP8266WiFi.h>
#include <Servo.h>
#include <DHT.h>
#define DHTPIN D1
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

float t = 0;
float h = 0;
float moisture = 0;
int M;
int CHALKYSOIL;
int REDSOIL;
int SILTYSOIL;

const char* ssid     = "your SSID";
const char* password = "your password";

const char* host = "api.thingspeak.com";
const String channelsAPIKey = "thingspeak write API";
const String talkBackAPIKey = "talkback API";
const String talkBackID = "talkback ID";
const unsigned int getTalkBackInterval = 10 * 1000;
const unsigned int updateChannelsInterval = 15 * 1000;
String talkBackCommand;
// capteurs


WiFiClient client;
Servo myservo;

long lastConnectionTimeChannels = 0;
boolean lastConnectedChannels = false;
int failedCounterChannels = 0;
long lastConnectionTimeTalkBack = 0;
boolean lastConnectedTalkBack = false;
int failedCounterTalkBack = 0;


void setup() {
  Serial.begin(115200);
  myservo.attach(D0);
  myservo.write(0);
  delay(10);

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  
  pinMode(D2, OUTPUT);
  startWiFi();
  
}

void loop()
{
h =dht.readHumidity(); 
t =dht.readTemperature();
    
M = analogRead(A0);
moisture = map(M,0,1023,0,100);

Serial.print("connecting to ");
Serial.println(host);

//client.stop();
delay(1000);
getTalkBack();
delay(5000);
updateChannels();

if (talkBackCommand=="CHALKYSOIL"){
myservo.write(90);
digitalWrite(D2, HIGH);
}
else if(talkBackCommand=="REDSOIL"){
myservo.write(45);
digitalWrite(D2, HIGH);

}
else if(talkBackCommand=="SILTYSOIL"){
myservo.write(0);
digitalWrite(D2, LOW);
}
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
client.println("GET /talkbacks/7136/commands/execute?api_key=WYZPG4ACYW1EAX7N");
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
tsData="field1="+String(t, DEC)+"&field2="+String(h, DEC)+"&field3="+String(moisture, DEC);
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
