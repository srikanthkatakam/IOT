#include <ESP8266WiFi.h>

const char* ssid     = "your SSID";
const char* password = "password";

const char* host = "host name";

String firstZigbee;
String SecondZigbee;

void setup() {
  Serial.begin(9600);

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

}

void loop() {
 
Serial.print("connecting to ");
Serial.println(host);
delay(1000);

// Use WiFiClient class to create TCP connections
WiFiClient client;
const int httpPort = 8080;
if (!client.connect(host, httpPort)) {
 Serial.println("connection failed");
 return;
}

// We now create a URL for the request
if (Serial.available()>0){    
String c = Serial.readString();
//Serial.println(c);
 firstZigbee = c.substring(16,46);
// SecondZigbee = c.substring(91,147);
  Serial.print("router1:");
  Serial.println(firstZigbee);
   //Serial.print("router2:");
 // Serial.println(SecondZigbee);

}
 //delay(4000);
//------------------------------------------------------------------

String tsData;
//tsData="/API/update?json1="+firstZigbee;
tsData = "{'temp':35}";
if (client.connect("nodefire-201ec.firebaseio.com", 80))
{
client.print("POST "+tsData+" HTTP/1.1\n");
client.print("Host:https://nodefire-201ec.firebaseio.com/sandeep\n");
client.println("Content-Type: application/json;charset=utf-8");
client.print("Connection: close\n");
client.print("Content-Type: application/json\n");
client.print("Content-Length: ");
client.print(tsData.length());
client.print("\n\n");
client.print(tsData);
      
               
delay(1000);
  
// Read all the lines of the reply from server and print them to Serial
while(client.available()){
String line = client.readStringUntil('\r');
Serial.print(line);
}
Serial.println();
Serial.println("closing connection");
}
}
