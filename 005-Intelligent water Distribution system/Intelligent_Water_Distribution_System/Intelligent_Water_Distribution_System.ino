// Intelligent Water Distribution System
#include <SPI.h>
#include <Ethernet.h>
#include <Servo.h>

Servo Valve1, Valve2;
#define trigPin 7
#define echoPin 8

volatile int flow_frequency1,flow_frequency2; // Measures flow sensor pulses
float l_hour1,l_hour2; // Calculated litres/hour
unsigned char flowsensor1 = 2; // Sensor Input
unsigned char flowsensor2 = 3; // Sensor Input
int flowstatus1;
int flowstatus2;
//unsigned long currentTime;
//unsigned long cloopTime;
unsigned int duration, level;

// Local Network Settings

byte mac[] = { 0xD4, 0x28, 0xB2, 0xFF, 0xA0, 0xA1 };

// ThingSpeak Settings
const String channelsAPIKey = "your thingspeak API";
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

EthernetClient client; // Initialize Arduino Ethernet Client


void flow () // Interrupt function
{
   flow_frequency1++;
   flow_frequency2++;
}


void setup()
{
  Serial.begin(9600); // Start Serial for debugging on the Serial Monitor
  startEthernet(); // Start Ethernet on Arduino

  Valve1.attach(5);
  Valve2.attach(6);
  
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  pinMode(flowsensor1, INPUT);
  pinMode(flowsensor2, INPUT);
  
  digitalWrite(flowsensor1, HIGH); // Optional Internal Pull-Up
  digitalWrite(flowsensor1, HIGH);
 
  
  //sei(); // Enable interrupts
  //currentTime = millis();
  //cloopTime = currentTime;
}

void loop()

{

//-------------------Tank Level----------------------- 
  digitalWrite(trigPin, HIGH);
  delay(1000);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  level =(duration/2) / 29.1;
  
//-------------------Flow Sensor-1---------------------
   attachInterrupt(0, flow, RISING); // Setup Interrupt
   flow_frequency1 =0;
   sei();
   delay(250);
   cli();
   detachInterrupt(0);
   l_hour1 = (flow_frequency1 * 60 / 7.5);
   if(l_hour1 >=95)
   {
    Serial.println(flowstatus1=1);
    
   }
   else
   {
   Serial.println(flowstatus1=0);
   }
   
//--------------------Flow Sensor-2---------------------
   attachInterrupt(1, flow, RISING); // Setup Interrupt
   flow_frequency2 =0;
   sei();
   delay(250);
   cli();
   detachInterrupt(1);
   l_hour2 = (flow_frequency2 * 60 / 7.5);
    if(l_hour2 >=95)
   {
    Serial.println(flowstatus2=1);
    
   }
   else
   {
   Serial.println(flowstatus2=0);
   }
   Serial.println(l_hour2);  
   
//-----------------Call to THingspeak --------------------------
delay(1000);
getTalkBack();
delay(1000);
updateChannels();

if(talkBackCommand == "OPEN1") Valve1.write(170);
if(talkBackCommand == "CLOSE1") Valve1.write(5);
if(talkBackCommand == "OPEN2") Valve2.write(170);
if(talkBackCommand == "CLOSE2") Valve2.write(5);

}

//--------------------Execute TalkBack Commands ------------------------------------

void getTalkBack()
{
String tsData="";
tsData=talkBackID + "/commands/execute?api_key=" + talkBackAPIKey;

if((!client.connected() && (millis() - lastConnectionTimeTalkBack > getTalkBackInterval)))
{
if (client.connect("api.thingspeak.com", 80))
{
//client.println("GET /talkbacks/"+tsData+" HTTP/1.0");
client.println("GET /talkbacks/7323/commands/execute?api_key=SUHXX1FYQDJN18UU");

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

//-----------------------------Update Parameters to ThingSpeak ------------------------------------------------------------
void updateChannels()
{
String tsData;
tsData="field1="+String(l_hour1, DEC)+"&field2="+String(l_hour2, DEC)+"&field3="+String(level, DEC)+"&field4="+String(flowstatus1, DEC)+"&field5="+String(flowstatus2, DEC);
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



//------------------Get an IP for the Device-------------------
void startEthernet()
{
  client.stop();
  Serial.println("Connecting Arduino to network...");
  Serial.println();  
  delay(1000);
    
    // Connect to network amd obtain an IP address using DHCP
  if (Ethernet.begin(mac) == 0){
    Serial.println("DHCP Failed, reset Arduino to try again");
    Serial.println();
    }
  
  else{
    Serial.println("Arduino connected to network using DHCP");
    Serial.println(Ethernet.localIP());
    Serial.println();
  }
  delay(2000);
}
