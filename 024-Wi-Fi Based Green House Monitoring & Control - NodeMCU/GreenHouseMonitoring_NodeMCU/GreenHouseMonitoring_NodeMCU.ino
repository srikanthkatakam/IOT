/*
 *  This sketch sends data via HTTP GET requests to api.thingspeak.com service.
 *
 *  You need to get API write Key at api.thingspeak.com  and paste them
 *  below. Or just customize this script to talk to other HTTP servers.
 *
 */

#include <ESP8266WiFi.h>
 
 #define trigPin 2
 #define echoPin 3
 #define led1 10
 #define led2 9 
int distance;

const char* ssid     = "your SSID";
const char* password = "password";

const char* host = "https://console.thinger.io";
const String channelsAPIKey = "thinger API";
const unsigned int updateChannelsInterval = 15 * 1000;

WiFiClient client;

long lastConnectionTimeChannels = 0;
boolean lastConnectedChannels = false;
int failedCounterChannels = 0;



void setup() {
  Serial.begin(115200);
 pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
 Serial.print("Ultrasonic Sensor Starting!!!");
  Serial.println("");
  delay(1000);

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  startWiFi();
  
}

void loop()
{
  int duration, distance;
  digitalWrite(trigPin, HIGH);
  delay(1000);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance =(duration/2) * 0.0343;// time * speed of sound(0.0343cm/microsec)
  if (distance < 8)
  {
    digitalWrite(led1, LOW);
    digitalWrite(led2, HIGH);
  }
  else
  {
    digitalWrite(led1, HIGH);
    digitalWrite(led2, LOW);
  }
   if (distance >= 150)
  {
    Serial.print("Out of Range");
    Serial.println("");
    
    
  }
  else
  {
  Serial.print(distance);
  Serial.println(" cm");
  delay(1000);
  }

Serial.print("connecting to ");
Serial.println(host);

//client.stop();
delay(5000);
updateChannels();

}

  
void updateChannels()
{
String tsData;
tsData="field1="+String(distance, DEC);
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
Serial.println("Connecting NodeMCU to network...");
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
