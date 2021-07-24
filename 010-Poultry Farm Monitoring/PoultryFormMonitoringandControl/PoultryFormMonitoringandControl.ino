#include "SPI.h"
#include "Ethernet.h"
#include "DHT.h"
#define DHTPIN 2     
#define DHTTYPE DHT11   
DHT dht (DHTPIN, DHTTYPE);

#define trigPin 8
#define echoPin 9

int duration, distance;

byte mac[] = { 0xD4, 0x28, 0xB2, 0xFF, 0xA0, 0xA1};
const String channelsAPIKey = "thingspeak write API";
const String talkBackAPIKey = "talkback API";
const String talkBackID = "talkback ID";

const unsigned int getTalkBackInterval = 10 * 1000;
const unsigned int updateChannelsInterval = 15 * 1000;
String talkBackCommand;

long lastConnectionTimeChannels = 0;
boolean lastConnectedChannels = false;
int failedCounterChannels = 0;
long lastConnectionTimeTalkBack = 0;
boolean lastConnectedTalkBack = false;
int failedCounterTalkBack = 0;

// capteurs
float temperature = 0;
float humidity = 0;
float Illumination = 0;
int I;
//int distance;
EthernetClient client;
void setup()
{
Serial.begin(9600);
pinMode(5, OUTPUT);
pinMode(6, OUTPUT);

pinMode(trigPin, OUTPUT);
pinMode(echoPin, INPUT);
  
startEthernet();
}


void loop()
{
humidity = dht.readHumidity(); 
temperature = dht.readTemperature(); 
I =  analogRead(A0);
Illumination = map(I,0,1023,0.1,1000);


digitalWrite(trigPin, HIGH);
delay(1000);
digitalWrite(trigPin, LOW);
duration = pulseIn(echoPin, HIGH);  
distance =(duration/2) * 0.0343;

//client.stop();
delay(1000);
getTalkBack();
delay(1000);
updateChannels();

if(talkBackCommand == "LIGHTON") digitalWrite(5, HIGH);
if(talkBackCommand == "LIGHTOFF") digitalWrite(5, LOW);
if(talkBackCommand == "AIRBLOWERON") digitalWrite(6, HIGH);
if(talkBackCommand == "AIRBLOWEROFF")digitalWrite(6, LOW);
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
client.println("GET /talkbacks/7139/commands/execute?api_key=talkback API");

Serial.println(tsData);
Serial.println("GET /talkbacks/"+tsData+" HTTP/1.0");
client.println();
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
while(client.connected() && !client.available()) delay(1); //waits for data
while (client.connected() || client.available())
{
char charIn = client.read();
Serial.print(charIn);
talkBackCommand += charIn;
}
//talkBackCommand=talkBackCommand.substring(talkBackCommand.indexOf("open")+5);
Serial.println();
Serial.println();
Serial.println("...disconnected");
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
if (failedCounterTalkBack > 3 ) {startEthernet();}
client.stop();
Serial.flush();
}


void updateChannels()
{
String tsData;
tsData="field1="+String(humidity)+"&field2="+String(temperature)+"&field3="+String(Illumination)+"&field4="+String(distance);
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
if (failedCounterChannels > 3 ) {startEthernet();
}
client.stop();
Serial.flush();
}
void startEthernet()
{
client.stop();
Serial.println();
//Serial.println();
//Serial.println("****************************************");
Serial.println("Connecting Arduino to network...");
delay(1000);
if (Ethernet.begin(mac) == 0)
{
//Serial.println("DHCP Failed, reset Arduino to try again");
//Serial.println("****************************************");
//Serial.println();
}
else
{
Serial.println("Arduino connected to network using DHCP");
Serial.println(Ethernet.localIP());
Serial.println();
}
delay(1000);
}
