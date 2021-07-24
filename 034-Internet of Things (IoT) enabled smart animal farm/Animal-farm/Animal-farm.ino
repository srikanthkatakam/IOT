// LIBRARY DECLARATION 
#include <DHT.h>  
#include <Ethernet.h>
#include <SPI.h>
//#include <Servo.h>
// CREATING AN INSTANCE FOR ultrasoniic
#define trigpin 5 // what pin we're connected to
#define echopin 6
#define AC 10
#define LIGHT 8
#define BLOWER 9
#define LDR A1
// CREATING AN INSTANCE FOR dht
#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN,DHTTYPE);

#define sensor A0
int dur, dis;
int gas_value;
byte mac[] = { 0xD4, 0x28, 0xB2, 0xFF, 0xA0, 0xA1 };// Must be unique on local network
const char* host = "Host Name";
const char* privateKey = "Host API";

// Initialize Arduino Ethernet Client
EthernetClient client;
void setup()
    {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("DHT11 test");
  dht.begin();
  pinMode(trigpin,OUTPUT);
  pinMode(echopin,INPUT);
  pinMode(AC,OUTPUT);
  pinMode(BLOWER,OUTPUT);
  pinMode(sensor,INPUT);
  //myservo.attach(4);
    }
  void loop() 
    {
        delay(1000); 
        
            
                float h =dht.readHumidity();
                float t = dht.readTemperature();
                Serial.println("humidity");
                Serial.println(h);
                Serial.println("%\t");
                Serial.println("temperature");
                Serial.println(t);
                Serial.println("*c");
                double gamma=log(h/100)+((17.27*t)/(237.7+t));
                double dp=237.7*gamma/(17.27-gamma); 
                Serial.println("Dew point");
                Serial.println(dp);
                delay(1000);
        if (dp>21)
         {
             digitalWrite(AC,HIGH);
             digitalWrite(BLOWER,LOW);
             Serial.println("AC ON");
             delay(1000);
         }
       else if (dp>10 && dp<20)
         {
                digitalWrite(AC,LOW);
               digitalWrite(BLOWER,LOW);
               Serial.println("AC OFF AND BOLWER OFF");
               delay(1000);
         }
      else
        {
              digitalWrite(AC,LOW);
              digitalWrite(BLOWER,HIGH);
              Serial.println("BLOWER ON");
             delay(1000);
       }
       int MQ2=analogRead(A0);
       if (MQ2<150)
      {
        Serial.println("harmful gases occured");
      }
      else if (MQ2>150 && MQ2<1000)
      {
               Serial.println("LPG  gases occured"); 
      }
      else {
                 Serial.println("ISO BUTANK I-C4H10  gases occured"); 
      }
        
      int LDR=analogRead(A1);
       if (LDR<400)
          {
              digitalWrite(LIGHT,HIGH);
              Serial.println("LIGHT ON");
             delay(1000);
          }
      else 
          {
              digitalWrite(LIGHT,LOW);
              Serial.println("LIGHT ON");
             delay(1000);
          }


          Serial.print("connecting to ");
        Serial.println(host);
  
        // Use WiFiClient class to create TCP connections
        EthernetClient client;
        const int httpPort = 80;
        if (!client.connect(host, httpPort)) {
          Serial.println("connection failed, Ethernet Restarted");  
          return;
            }
  // We now create a URL for the request
 
  String url = "Host Name";
  url += "?api_key=";
  url += privateKey;
  url += "&field1=";
  url += t;
  url += "&field2=";
  url += h;
  url += "&field3=";
  url += dp;
  url += "&field4=";
  url += gas_value;
  url += "&field5=";
  url += dis;
  url += "&field6=";
  url += LDR;
   delay(5000);


  Serial.print("Requesting URL: ");
  Serial.print(url);
 client.print(String("GET ") + url + " HTTP/1.1\r\n" +
              "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  delay(10);
  
  // Read all the lines of the reply from server and print them to Serial
  if(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
  
  Serial.println();
  Serial.println("closing connection");
 // Condition();
}
