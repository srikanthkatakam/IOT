#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include<Servo.h>

Servo myservo;

#define Switch D8
#define led1 D0
#define led2 D1
#define led3 D2
#define mq_6 A0
#define pir_sensor1 D3
#define pir_sensor2 D5
#define pir_sensor3 D6

void reconnect();
void MQpublish();
String cmd;
void pir_sensor();

// Update these with values suitable for your network.

const char* ssid = "SSID";
const char* password = "password";
const char* mqtt_server = "broker.mqtt-dashboard.com";

WiFiClient espClient;
PubSubClient client(espClient);

void setup() 
{
  
  Serial.begin(9600);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
 client.setCallback(callback);
 myservo.attach(D7);
 pinMode(led1,OUTPUT);
 pinMode(led2,OUTPUT);
 pinMode(led3,OUTPUT);
 pinMode(mq_6,INPUT);
 pinMode(pir_sensor1,INPUT);
 pinMode(pir_sensor2,INPUT);
 pinMode(pir_sensor3,INPUT);
 
}

void loop() {
       delay(100);
  if (!client.connected())
  {
   reconnect();
  }
  
  client.loop();
  cmd ="";

MQpublish();
delay(2000);
pir_sensor();
  
}

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid,password);

  while (WiFi.status()!= WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length)
{

  int  Value = digitalRead(D2);

 if (Value == HIGH)
{ 
  client.publish("SmartHomeStatus","HIGH");
  delay(2000);
  client.publish("SmartHomeStatus", "some one arrived");
}
  Serial.print("Message arrived[");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) 
  {
    //Serial.print((char)payload[i]);
    cmd = cmd + String((char)payload[i]);
  }
  Serial.println(cmd);
  
  // Switch on the LED if an 1 was received as first character
  if (cmd == "open") 
  {
    delay(20);
    myservo.write(180);
      // Turn the LED on (Note that LOW is the voltage level
    Serial.println("gate is open");
    client.publish("SmartHomeStatus", "gate is open");
    } 
  else if (cmd == "Close")
  {
    delay(500);
    myservo.write(0);
    client.publish("SmartHomeStatus", "gate is closed");
  }
  else {
    Serial.println("Waiting for Command");
  }
 
  cmd ="";

}
void reconnect() 
{
  // Loop until we're reconnected
  while (!client.connected())
  {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("espClient")) {
      Serial.println("connected");
      client.subscribe("SmartHome");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
void pir_sensor()

{
   int pir1status= digitalRead(D3);
   
   Serial.println(pir1status);
   if (pir1status==1)
  {
    
    
    digitalWrite(led1,LOW);
    Serial.println("Motion detected");
    
    }
    
  else 
  
  {
    
    digitalWrite(led1,HIGH);
    Serial.println("No motion detected");
    
  }
  
    delay(2000);
   int pir2status= digitalRead(D5);
    if (pir2status ==HIGH)
  {
    
    digitalWrite(led2,LOW);
     Serial.println("Motion detected");
   
    
  }
  else
  {
    
    digitalWrite(led2,HIGH);
    Serial.println("No motion detected");
    
    
  }
   
   int pir3status= digitalRead(D6);
   
   if (pir3status ==HIGH)
  {
    delay(2000);
    digitalWrite(led3,LOW);
     Serial.println("Motion detected");
    
    
  }
  else 
  {
    
    digitalWrite(led3,HIGH);
    Serial.println("No motion detected");
    
  }
}

void MQpublish()
{
  char str[20];
float gas_value =analogRead(A0);
 sprintf(str,"%d",gas_value);
  if(gas_value >300)
  {
    
    client.publish("SmartMonitor","Alert!! combustible gas detected");
    delay(2000);
     client.publish("SmartMonitor",str); 
     
  }
  else
  {
    client.publish("SmartMonitor","All is Good");
     delay(2000);
    client.publish("SmartMonitor",str);
  }

}
