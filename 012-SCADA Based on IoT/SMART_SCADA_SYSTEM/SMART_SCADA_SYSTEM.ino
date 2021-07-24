#include "SPI.h"
#include "Ethernet.h" 
byte mac[] = { 0xD4, 0x28, 0xB2, 0xFF, 0xA0, 0xA1};
IPAddress ip(192,168,0,190);
IPAddress gateway(192,168,0,1);
IPAddress subnet(255, 255, 255, 0);

const String channelsAPIKey = "thinspeak write API";
const String talkBackAPIKey = "talkback API";
const String talkBackID = "talkback ID";
//const String talkCommandID = "880921";
const unsigned int getTalkBackInterval = 10 * 1000;
const unsigned int updateChannelsInterval = 15 * 1000;
String talkBackCommand;

long lastConnectionTimeChannels = 0;
boolean lastConnectedChannels = false;
int failedCounterChannels = 0;
long lastConnectionTimeTalkBack = 0;
boolean lastConnectedTalkBack = false;
int failedCounterTalkBack = 0;
int V;
int i;
int Current = A0;
int Voltage = A1;
const int CB  = 3;
const int ISO1 = 5;
const int ISO2  = 6;
int CBstatus  = 0;
int ISO1status = 0;
int ISO2status = 0;



// capture
EthernetClient client;
void setup()
{
Serial.begin(9600);
pinMode(CB,  OUTPUT);
pinMode(ISO1, OUTPUT);
pinMode(ISO2,  OUTPUT);
startEthernet();
}
void loop()
{
Current = ((float)analogRead((A0)-511)*5)/(0.1*1024);
V =  (analogRead(A1),DEC);
Voltage = map(V,0,1023,0,230);
//client.stop();
delay(1000);
getTalkBack();
delay(1000);
updateChannels();

//interlock between ISO's and CB

if ((ISO1status == 1) && (ISO2status==1) && (talkBackCommand=="CBON")){
digitalWrite(CB, HIGH);
CBstatus = 1;
}
else if(talkBackCommand=="CBOFF"){
digitalWrite(CB, LOW);
CBstatus =0;
}
else if((CBstatus==0) && (talkBackCommand=="ISO1ON")){
digitalWrite(ISO1, HIGH);
ISO1status=1;
}
else if((CBstatus==0) && (talkBackCommand=="ISO1OFF")){
digitalWrite(ISO1, LOW);
ISO1status=0;
}
else if((CBstatus==1) && (talkBackCommand=="ISO1ON")){
Serial.println("CB is in Closed Position: ISO1 ON Prohibited");
}
else if ((CBstatus==1) && (talkBackCommand=="ISO1OFF")){
Serial.println("CB is in Closed Position: ISO1 OFF Prohibited");
}
else if((CBstatus==0) && (talkBackCommand=="ISO2ON")){
digitalWrite(ISO2, HIGH);
ISO2status=1;
}
else if((CBstatus==0) && (talkBackCommand=="ISO2OFF")){
digitalWrite(ISO2, LOW);
ISO2status=0;
}
else if((CBstatus==1) && (talkBackCommand=="ISO2ON")){
Serial.println("CB is in Closed Position: ISO2 ON Prohibited");
}
else if((CBstatus==1) && (talkBackCommand=="ISO2OFF")){
Serial.println("CB is in Closed Position: ISO2 OFF Prohibited");
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
client.println("GET /talkbacks/6987/commands/execute?api_key=E9VS5XABK28AG44T");

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
tsData="field1="+String(Current, DEC)+"&field2="+String(Voltage, DEC)+"&field3="+String(CBstatus, DEC)+"&field4="+String(ISO1status, DEC)+"&field5="+String(ISO2status, DEC);
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
Serial.println("Uptade channels");
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
Serial.println();
Serial.println("****************************************");
Serial.println("Connecting Arduino to network...");
delay(1000);
if (Ethernet.begin(mac) == 0)
{
Serial.println("DHCP Failed, reset Arduino to try again");
Serial.println("****************************************");
Serial.println();
}
else
{
Serial.println("Arduino connected to network using DHCP");
Serial.println(Ethernet.localIP());
Serial.println();
}
delay(1000);
}
