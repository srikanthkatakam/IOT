#include "SPI.h"
#include "Ethernet.h"
 #include "Servo.h"
#include <HX711.h>

#define DOUT A3
#define CLK A2
HX711 scale(DOUT, CLK);

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
float modWeight;
float LpgGas;

// capteurs

EthernetClient client;
Servo myservo;

float calibration_factor= -182530;

void setup()
{
Serial.begin(9600);
myservo.attach(9); 

scale.set_scale();
scale.tare(); //reset the scale to 0

startEthernet();
}


void loop()
{
// Read value of TotalWeight from Analog Input Pin A2 & A3
  scale.set_scale(calibration_factor);
  float value=scale.get_units()*453;
  modWeight = value;
  Serial.println(modWeight);

  // Read Value of LPG sensor from Analog Input Pin A0
  
  LpgGas = analogRead(A0);
  Serial.println(LpgGas);
  delay(1000);

//client.stop();
delay(1000);
getTalkBack();
delay(1000);
updateChannels();

if(talkBackCommand == "0") myservo.write(0);
if(talkBackCommand == "30") myservo.write(30);
if(talkBackCommand == "60") myservo.write(60);
if(talkBackCommand == "90") myservo.write(90);
if(talkBackCommand == "120") myservo.write(120);
if(talkBackCommand == "150") myservo.write(150);
if(talkBackCommand == "180") myservo.write(180);
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
client.println("GET /talkbacks/7004/commands/execute?api_key=EXP2QHWC84MBSN6J");

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
tsData="field1="+String (modWeight, DEC)+"&field2="+String (LpgGas, DEC);
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
if (failedCounterChannels > 3 ) {startEthernet();}
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
