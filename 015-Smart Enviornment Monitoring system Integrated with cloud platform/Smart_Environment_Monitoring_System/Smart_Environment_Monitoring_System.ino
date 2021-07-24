/*

 Arduino --> ThingSpeak Channel via Ethernet
 
 The ThingSpeak Client sketch is designed for the Arduino and Ethernet.
 This sketch updates a channel feed with an analog input reading via the
 ThingSpeak API (https://thingspeak.com/docs)
 using HTTP POST. The Arduino uses DHCP and DNS for a simpler network setup.
 The sketch also includes a Watchdog / Reset function to make sure the
 Arduino stays connected and/or regains connectivity after a network outage.
 Use the Serial Monitor on the Arduino IDE to see verbose network feedback
 and ThingSpeak connectivity status.
 
 Getting Started with ThingSpeak:
 
   * Sign Up for New User Account - https://thingspeak.com/users/new
   * Create a new Channel by selecting Channels and then Create New Channel
   * Enter the Write API Key in this sketch under "ThingSpeak Settings"
 
 Arduino Requirements:
 
   * Arduino with Ethernet Shield or Arduino Ethernet
   * Arduino 1.0+ IDE
   
  Network Requirements:

   * Ethernet port on Router    
   * DHCP enabled on Router
   * Unique MAC Address for Arduino
*/

#include <SPI.h>
#include <Ethernet.h>
#include "DHT.h"
#define DHTPIN 2     // what pin we're connected to
#define DHTTYPE DHT11   // define type of sensor DHT 11
DHT dht (DHTPIN, DHTTYPE);
int v;
int Sound;

// Local Network Settings
byte mac[] = { 0xD4, 0x28, 0xB2, 0xFF, 0xA0, 0xA1 }; // Must be unique on local network

// ThingSpeak Settings
char thingSpeakAddress[] = "api.thingspeak.com";
String writeAPIKey = "thingspeak write API";
const int updateThingSpeakInterval = 16 * 1000;      // Time interval in milliseconds to update ThingSpeak (number of seconds * 1000 = interval)

// Variable Setup
long lastConnectionTime = 0; 
boolean lastConnected = false;
int failedCounter = 0;

EthernetClient client; // Initialize Arduino Ethernet Client
IPAddress ip(192,168,0,190);
IPAddress gateway(192,168,0,1);
IPAddress subnet(255, 255, 255, 0);


void setup()
{
  Serial.begin(9600); // Start Serial for debugging on the Serial Monitor
  startEthernet(); // Start Ethernet on Arduino
  Ethernet.begin(mac, ip, gateway, subnet);
  
  dht.begin();

}

void loop()
{
  //------------------------------------------------------------------------
  // Read value of Humidity Sensor from Digital Pin 2
  delay(2000);
  String h = String (dht.readHumidity(), DEC);
  String t = String (dht.readTemperature(), DEC);

  /*if (isnan(h) || isnan(t))
  {
  Serial.println("Failed to read from DHT sensor!");
  return;
  }
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.println(" *C ");*/

  //--------------------------------------------------------------------------
  
  // Read Value of MQ-135 from Analog Input Pin A0
  
  String MQ135 = String (analogRead(A0), DEC);
  //Serial.println(" MQ135 Value: ");
  //Serial.println(MQ135);

  //--------------------------------------------------------------------------
  
  // Read Value of MQ-7 from Analog Input Pin A1
  
  String CO = String (analogRead(A1), DEC); 
  //Serial.println(" MQ7 Value: ");
  Serial.println(CO);
//-----------------------------------------------------------------------------

  // Read Value of Sound Sensor from Analog Input Pin A3
  
  int v = analogRead(A3); // Reads the value from the Analog PIN A3
  Sound=  map(v,0,1023,50,140);

   Serial.println(Sound);
  
  // Print Update Response to Serial Monitor
  if (client.available())
  {
    char c = client.read();
    client.stop();
    Serial.print(c);
    
  }

  // Disconnect from ThingSpeak
  if (!client.connected() && lastConnected)
  {
    Serial.println("...disconnected");
    Serial.println();
    
    client.stop();
  }
  
  // Update ThingSpeak
  if(!client.connected() && (millis() - lastConnectionTime > updateThingSpeakInterval))
  {
    updateThingSpeak("field1="+h+"&field2="+t+"&field3="+MQ135+"&field4="+CO+"&field5="+String (Sound));
  }
  
  // Check if Arduino Ethernet needs to be restarted
  if (failedCounter > 3 ) {startEthernet();}
  
  lastConnected = client.connected();
  
}

void updateThingSpeak(String tsData)
{
  if (client.connect(thingSpeakAddress, 80))
  {         
    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: "+writeAPIKey+"\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(tsData.length());
    client.print("\n\n");

    client.print(tsData);
    EthernetServer server(80); 
    lastConnectionTime = millis();
    
    if (client.connected())
    {
      Serial.println("Connecting to ThingSpeak...");
      Serial.println();
      
      failedCounter = 0;
    }
    else
    {
      failedCounter++;
  
      Serial.println("Connection to ThingSpeak failed ("+String(failedCounter, DEC)+")");   
      Serial.println();
    }
    
  }
  else
  {
    failedCounter++;
    
    Serial.println("Connection to ThingSpeak Failed ("+String(failedCounter, DEC)+")");   
    Serial.println();
    
    lastConnectionTime = millis(); 
  }
}

void startEthernet()
{
  
  client.stop();

  Serial.println("Connecting Arduino to network...");
  Serial.println();  

  delay(1000);
  
  // Connect to network amd obtain an IP address using DHCP
  if (Ethernet.begin(mac) == 0)
  {
    Serial.println("DHCP Failed, reset Arduino to try again");
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
