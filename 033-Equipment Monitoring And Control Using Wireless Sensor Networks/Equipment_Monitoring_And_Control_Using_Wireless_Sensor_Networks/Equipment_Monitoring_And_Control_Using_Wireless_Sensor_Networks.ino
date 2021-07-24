#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include <Servo.h>


SoftwareSerial mySerial(D5,D6);

const char* ssid     = "your SSID";
const char* password = "password";
const char* host = "api.thingspeak.com";
const char* privateKey = "thingspeak write API";
const String talkBackAPIKey = "talkback API";
const String talkBackID = "talkback ID";
const unsigned int getTalkBackInterval = 10 * 1000;

String talkBackCommand;
long lastConnectionTimeChannels = 0;
boolean lastConnectedChannels = false;
int failedCounterChannels = 0;
long lastConnectionTimeTalkBack = 0;
boolean lastConnectedTalkBack = false;
int failedCounterTalkBack = 0;
WiFiClient client;
int temperature,temp,vibration;
int tMSB,tLSB,vibMSB,vibLSB;


String readString;
Servo myservo;

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
mySerial.begin(9600);
myservo.attach(D2);
myservo.write(0);
 delay(10);

  // We start by connecting to a WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
 startWiFi();
}

void loop() { 
readValues();
getTalkBack();
delay(1000);
if(talkBackCommand == "MOTORON") myservo.write(180);
if(talkBackCommand == "MOTOROFF") myservo.write(0);



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
  url += temperature;
  url += "&field2=";
  url += vibration;



  
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
 


void readValues()
{
if(mySerial.available()>=25){
if(mySerial.read() == 0x7E){
  for (int i=1;i<20;i++) {
    byte discardByte = mySerial.read();
   // Serial.println(discardByte);
  }
  tMSB = mySerial.read();
  tLSB = mySerial.read();
  temp= tLSB + (tMSB * 256);
 // Serial.println(moisture);
   temperature=(5*temp*100)/1024;//Converting digital value to analog value
  Serial.println("temperature:");
  Serial.println(temperature);
  vibMSB = mySerial.read();
  vibLSB = mySerial.read();
  vibration= vibLSB + (vibMSB * 256);
  Serial.println("vibration:");
  Serial.println(vibration);
  delay(1000);  
}
}
logic();
}
void logic()
{
  if((temperature>=30)&&(vibration<=500))
  
  { 
    Serial.println("servo on");
    myservo.write(180);
    delay(2000);
    
    }
    
  
  else 
  {Serial.println("servo off");
    myservo.write(0);
    delay(15);
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
client.println("GET /talkbacks/13346/commands/execute?api_key=TCCN1G2YIBKQLLPO");
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
void startWiFi()
{
client.stop();
Serial.println();
Serial.println("Connecting  to network...");
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
