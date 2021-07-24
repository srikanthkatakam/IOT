#include <Ethernet.h>
#include <SPI.h>
byte mac[] = {0xD4, 0x28, 0xB2, 0xFF, 0xA0, 0xA1 };// Must be unique on local network
const char* host = "api.thingspeak.com";
const char* privateKey = "thingspeak write key";

// Initialize Arduino Ethernet Client
EthernetClient client;

void startEthernet();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  digitalWrite(3, HIGH);
  digitalWrite(4, HIGH);
  digitalWrite(5, HIGH);
   startEthernet();

}

void loop() {
  // put your main code here, to run repeatedly:
float a = analogRead(A0);
float b = analogRead(A1);
float c = analogRead(A2);
float a1=map(a,0,1024,0,5);
float b1=map(b,0,1024,0,5);
float c1=map(c,0,1024,0,5);
Serial.print("a :");
Serial.println(a1);
Serial.print("b :");
Serial.println(b1);
Serial.print("c :");
Serial.println(c1);
if(4.5>a1>5)
{
  Serial.println("B1-Good condition");
}
else if(4.0>a1>4.5)
{
  Serial.println("B1-Normal condition");
}
else
{
  Serial.println("B1-extreme conditions");
}

if(4.5>b1>5)
{
  Serial.println("Good condition");
}
else if(4.0>b1>4.5)
{
  Serial.println("Normal condition");
}
else
{
  Serial.println("extreme conditions");
}

if(4.5>c1>5)
{
  Serial.println("B2-Good condition");
}
else if(4.0>c1>4.5)
{
  Serial.println("B2-Normal condition");
}
else
{
  Serial.println("B2-extreme conditions");
}

delay(1000);


        Serial.print("connecting to ");
        Serial.println(host);
  
        // Use WiFiClient class to create TCP connections
        EthernetClient client;
        const int httpPort = 80;
        if (!client.connect(host, httpPort)) {
          Serial.println("connection failed, Ethernet Restarted");  
          return;
          }
  String url = "/update";
  url += "?api_key=";
  url += privateKey;
  url += "&field1=";
  url += a1;
  url += "&field2=";
  url += b1;
  url += "&field3=";
  url += c1;
 


  Serial.print("Requesting URL: ");
  Serial.print(url);
 client.print(String("GET ") + url + " HTTP/1.1\r\n" +
              "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  delay(2000);
  
  // Read all the lines of the reply from server and print them to Serial
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
  
  Serial.println();
  Serial.println("closing connection");
          
}

void startEthernet(){ 
 

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
    Serial.println(Ethernet.localIP());
    Serial.println("Arduino connected to network using DHCP");
    Serial.println();
  }
  
  delay(1000);
}
