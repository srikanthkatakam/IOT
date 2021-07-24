#include <SoftwareSerial.h>
SoftwareSerial myGPRS(10,11);
#include "HX711.h"

#define trigPin 12
#define echoPin 11
#define DOUT  A3
#define CLK  A2

HX711 scale(DOUT, CLK);

float calibration_factor = 182540; 
String Message;
int d;
 int duration, distance, binweight; // Duration used to calculate distance

String host= "api.thingspeak.com";

String GET = "GET /update?key=V9YZJIIMRTWG09J9";

void setup()  
{
 Serial.begin(115200);
 myGPRS.begin(115200);
 delay(1000);
 pinMode(trigPin, OUTPUT);
 pinMode(echoPin, INPUT);

 scale.set_scale();
 scale.tare();  //Reset the scale to 0

 long zero_factor = scale.read_average(); //Get a baseline reading
 Serial.print("Zero factor: "); //This can be used to remove the need to tare the scale. Useful in permanent scale projects.
 Serial.println(zero_factor);

  sendmyGPRS("AT");
  delay(5000);
  
  if(myGPRS.find("OK")) 
  {
      d = myGPRS.read();
      //Serial.println(d);
  }

}

void loop() // run over and over
{

 digitalWrite(trigPin, HIGH); 
 delay(1000); 
 digitalWrite(trigPin, LOW);
 duration = pulseIn(echoPin, HIGH);
 distance = (duration/2) * 0.0343;
 Serial.print("distance=");
 Serial.println(distance);
 
 scale.set_scale(calibration_factor); //Adjust to this calibration factor

  binweight=scale.get_units()*453;
  
  Serial.print("Reading: ");
  Serial.print(binweight);
  Serial.println(" grams");
  
  if (distance<20){
      Message = "Bin filled";
      SendMessage(Message); 
  }

 if (binweight>2000){
      Message = "Bin overload";
      SendMessage(Message); 
  }

uploadtoThingspeak();
delay(15000);
}

void SendMessage(String text)
{
  myGPRS.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);  // Delay of 1000 milli seconds or 1 second
  myGPRS.println("AT+CMGS=\"+919676028012\"\r"); // Replace x with mobile number
  delay(1000);
  myGPRS.println(text);// The SMS text you want to send
  delay(100);
  myGPRS.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
}

void uploadtoThingspeak(){

myGPRS.listen();

sendmyGPRS("AT+CIPSHUT");
  delay(2000);
  if( myGPRS.find( "SHUT OK" ) )
  {
    
       d = myGPRS.read();
     Serial.write(d);
     //Serial.println(d);
    //return;
  }
  
sendmyGPRS("AT+CIPMUX=1");
  delay(2000);
 if( myGPRS.find( "OK" ) )
  {
       d = myGPRS.read();
      Serial.write(d);
      //Serial.println();
    //return;
  }
sendmyGPRS("AT+CGATT=1");
  delay(2000);

   if( myGPRS.find( "OK" ) )
  {
      d = myGPRS.read();
      Serial.write(d);
      //Serial.println(d);
    //return;
  }
  
String cmd = "AT+CSTT=\"Vodafone_gprs\",\"\",\"\"";// Setup TCP connection
sendmyGPRS(cmd);
  delay(2000);

   if( myGPRS.find( "OK" ) )
  {
      d= myGPRS.read();
     Serial.write(d);
     // Serial.println();
    //return;
  }

sendmyGPRS("AT+CIICR");
  delay(2000);

   if( myGPRS.find( "OK" ) )
  {
     d = myGPRS.read();
      Serial.write(d);
      //Serial.println();
    //return;
  }

sendmyGPRS("AT+CIFSR");
  delay(2000);
   
   if( myGPRS.find( "Error" ) )
  {
    myGPRS.println( "ERROR Received" );
    //return;
  }
  else {
    d = myGPRS.read();
    Serial.write(d);
     // Serial.println();
  }
  
  cmd = "AT+CIPSTART=3,\"TCP\",\"";// Setup TCP connection
  cmd += host;
  cmd += "\",80";

sendmyGPRS(cmd);
  delay(4000);

     if( myGPRS.find("CONNECTED") )
  {
      d = myGPRS.read();
      //Serial.write(d);
      Serial.println("Connected to Thingspeak");
    //return;
  }


String cmd1 = "AT+CIPSEND=3,100";// Setup TCP connection
sendmyGPRS(cmd1);
delay(2000);

cmd = GET+ "&field1="+distance+"&field2="+binweight+"\r\n";

if( myGPRS.find( ">" ) )
  {
    myGPRS.println(cmd);
    myGPRS.println();
    Serial.println(cmd);
  }
  else
  {
    sendmyGPRS( "AT+CIPCLOSE" );//close TCP connection
  }
  sendmyGPRS("AT+CIPCLOSE");
}


void sendmyGPRS(String cmd)
{
  myGPRS.println(cmd);
  //Serial.println(cmd);
}
