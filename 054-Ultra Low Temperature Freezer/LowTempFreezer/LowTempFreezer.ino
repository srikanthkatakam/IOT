#include <ESP8266WiFi.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS D2

OneWire oneWire(ONE_WIRE_BUS);

DallasTemperature sensors(&oneWire);

const char* ssid     = "ssid";//Enter the ssid of your router
const char* password = "password";//Enter the password of your router

const char* host = "api.thingspeak.com";
const char* privateKey = "thingspeak write key"; //place your write api key
float Celcius=0;

void setup() {
  Serial.begin(115200);
  delay(10);
  sensors.begin();
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
  //WiFiManager wifiManager;
  // We start by connecting to a WiFi network
 // Serial.println();
    
 // wifiManager.autoConnect("LowTempFreezer");
// Serial.println("WiFi connected"); 
}


void loop() 
{
  sensors.requestTemperatures(); 
  Celcius=sensors.getTempCByIndex(0);
  Serial.print(" C  ");
  Serial.println(Celcius);
  delay(15000);
  Upload_to_Cloud();  
}


/*.....................................Uploading to Cloud...............................*/

void Upload_to_Cloud()
{
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
  url += Celcius;
  
  
  Serial.print("Requesting URL: ");
  Serial.println(url);
  
  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  delay(1000);
  
  // Read all the lines of the reply from server and print them to Serial
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
  
  Serial.println();
  Serial.println("closing connection");
}
