#include <EEPROM.h>
#include <SPI.h>
#include <Ethernet.h>
int startcount=0;
int tripcount=0;
int btcount=0;
int wtcount=0;
int Current;
int Voltage;


// Local Network Settings
byte mac[] = { 0xD4, 0x28, 0xB2, 0xFF, 0xA0, 0xA1 }; // Must be unique on local network

// ThingSpeak Settings
char thingSpeakAddress[] = "api.thingspeak.com";
String writeAPIKey = "thingspeak API";
const int updateThingSpeakInterval = 16 * 1000;      // Time interval in milliseconds to update ThingSpeak (number of seconds * 1000 = interval)
// Variable Setup
long lastConnectionTime = 0; 
boolean lastConnected = false;
int failedCounter = 0;
// capteurs
EthernetClient client;
void setup()
{
Serial.begin(9600);
//put your setup code here, to run once:
  pinMode(2,INPUT);//Vibration Input
  pinMode(3,OUTPUT);
  pinMode(4,OUTPUT);//Motor Healthy Indication 
  pinMode(5,OUTPUT);//Warning Alarm
  pinMode(6,OUTPUT);//Emergency Alarm
  pinMode(7,INPUT);//Trip Feedback
  pinMode(8,INPUT);//Start Feedback
}
void loop() 
{
 // Temperature program:
  int temp=analogRead(A2); //Winding Temperature
  float windingtemp=(5.0*temp*100.0)/1024;
  Serial.print("Winding temperature= ");
  Serial.print(windingtemp);
  Serial.println("*C");
  
  int temp1=analogRead(A3);//Bearing Temperature
  float Bearingtemp=(5.0*temp1*100.0)/1024;
  Serial.print("Bearing temperature= ");
  Serial.print(Bearingtemp);
  Serial.println("*C");
  
  //Current Monitoring program:
  int i=analogRead(A0);
  Current = ((float)analogRead((A0)-511)*5)/(0.1*1024);
 
  Serial.print("Running Current= ");
  Serial.print(Current);
  Serial.println("A");
  
   //Voltage Monitoring program:
   int v=analogRead(A1);
   float voltage = map (v,0,1023,0,230);
   Serial.print("Supply Voltage= ");
   Serial.print(voltage);
   Serial.println("v");
  
   //Monitoring of Vibration levels
   
  long vibration=pulseIn (2, HIGH);
  delay(2000);
  Serial.print("Motor Vibration = ");
  Serial.println(vibration); 
   
   
   //Monitoring no. of starts
  int starts=EEPROM.read(8);//No. of Motor Starts
  EEPROM.write(startcount, starts);
  startcount = startcount + 1;
  if (startcount == EEPROM.length()) {
  startcount = 0;
  }
  Serial.print("No.of starts= ");
  Serial.println(starts);
   
   //Monitoring no. of trips
   int trips=EEPROM.read(7);
   EEPROM.write(tripcount, trips);
    tripcount = tripcount + 1;
   if(tripcount==EEPROM.length()){
    tripcount=0;
   
   }
   Serial.print("No.of Trips= ");
   Serial.println(trips);
 

   
 //Algorithm for Predictive maintenance
   if (startcount>=5){
    Serial.println("Start count exceeds limit");
    EEPROM.write(5,HIGH);
   } 
    
    if (tripcount>=5){
      Serial.println("Trip count exceeds limit");
      EEPROM.write(5,HIGH);
    }
      
      if (Bearingtemp>=24){
       btcount++;
        if (btcount>=5){
        Serial.println("Probable Bearing failure");
        digitalWrite(5,LOW);
        }
      }
        
      if (windingtemp>=24){
        wtcount++;
        if(wtcount>=5){
          Serial.println("Probable winding damage");
          digitalWrite(6, LOW);
        }
      }

      if (vibration>=2000){
        Serial.println("High Motor Vibration");
        digitalWrite(2,LOW);
      }
      Serial.println("");
      Serial.println("");
      Serial.println("");
      delay(2000);

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
    updateThingSpeak("field1="+String(Current, DEC)+"&field2="+String(voltage, DEC)+"&field3="+String(vibration, DEC)+"&field4="+String(windingtemp, DEC)+"&field5="+String(Bearingtemp, DEC)+"&field6="+String(startcount, DEC)+"&field7="+String(tripcount, DEC));
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
