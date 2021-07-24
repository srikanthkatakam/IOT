#include <ESP8266WiFi.h>
#include<SoftwareSerial.h>
#include <ArduinoJson.h>

SoftwareSerial xbee(D3, D4); //Rx, Tx
String input;
//long Flux,Vibration,Force;

// ThingSpeak Settings
const char* ssid     = "your SSID";
const char* password = "password";

const char* host = "api.thingspeak.com";
const char* privateKey = "thingspeak write key";
 WiFiClient client;
//  const int httpPort = 80;


void setup() {
  Serial.begin(9600);
  xbee.begin(9600);
  delay(10);
 
  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
DynamicJsonBuffer jsonBuffer;
if (xbee.available()>0){    
input = xbee.readStringUntil('\r');
Serial.println(input);
}
JsonObject& root = jsonBuffer.parseObject(input);

//String id = root[String("id")];
long Flux = root[String("Flux")];
 long  Vibration = root[String("Vibration")];
 long  Force = root[String("Force")];

// Serial.println("id:" +id);
 Serial.println("Xaxis:" +String(Flux));
Serial.println("Yaxis:" +String(Vibration));
Serial.println("Zaxis:" +String(Force));
delay(100);
Serial.print("connecting to ");
  Serial.println(host);

  const int httpPort = 80;

if (!client.connect(host, 80)) {
    Serial.println("connection failed");
    return;
  }
  
  // We now create a URI for the request
  String url = "/update";
  url += "?api_key=";
  url += privateKey;
  url += "&field1=";
  url += Flux;
  url += "&field2=";
  url += Vibration;
  url += "&field3=";
  url += Force;

  
  Serial.print("Requesting URL: ");
  Serial.println(url);
  
  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  delay(10);
  
  // Read all the lines of the reply from server and print them to Serial
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
  
  Serial.println();
  Serial.println("closing connection");
  
}
