// Smart Vehicle Tracking System

#include <TinyGPS.h>
#include <SoftwareSerial.h>

SoftwareSerial myGPS(5, 6); // RX, TX
SoftwareSerial myGPRS(10,11);

TinyGPS gps;
String host= "api.thingspeak.com";

void gpsdump(TinyGPS &gps);
//void printFloat(double f, int digits = 2);

String GET = "GET /update?key=thingspeak write API";
float flat,flon;
long lat, lon;
char d;

void setup()  
{
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  
  // set the data rate for the SoftwareSerial port
  myGPS.begin(9600);
  myGPRS.begin(9600);
  delay(1000);
  
  sendmyGPRS("AT");
  delay(5000);
  
  if(myGPRS.find("OK")) 
  {
      char d = myGPRS.read();
      Serial.println(d);
  }

}

void loop() // run over and over
{

myGPS.listen();
  
  bool newdata = false;
  unsigned long start = millis();


  // Every 5 seconds we print an update
  while (millis() - start < 5000) {
    if (myGPS.available()) {
      char c = myGPS.read();
      //Serial.print(c);  // uncomment to see raw GPS data
      if (gps.encode(c)) {
        newdata = true;
        // break;  // uncomment to print new data immediately!
      }
    }
  }
    
if (newdata) {
    Serial.println("Acquired GPS Data");
    Serial.println("-------------");
    gpsdump(gps);
    delay(1000);

  }  

uploadtoThingspeak();
delay(15000);
}


void uploadtoThingspeak(){

myGPRS.listen();

sendmyGPRS("AT+CIPSHUT");
  delay(2000);
  if( myGPRS.find( "SHUT OK" ) )
  {
    
      char d = myGPRS.read();
      Serial.write(d);
      //Serial.println();
    //return;
  }
  
sendmyGPRS("AT+CIPMUX=1");
  delay(2000);
 if( myGPRS.find( "OK" ) )
  {
      char d = myGPRS.read();
      Serial.write(d);
      //Serial.println();
    //return;
  }
sendmyGPRS("AT+CGATT=1");
  delay(2000);

   if( myGPRS.find( "OK" ) )
  {
      char d = myGPRS.read();
       Serial.write(d);
      //Serial.println();
    //return;
  }
  
String cmd = "AT+CSTT=\"airtelgprs.com\",\"\",\"\"";// Setup TCP connection
sendmyGPRS(cmd);
  delay(2000);

   if( myGPRS.find( "OK" ) )
  {
     char d= myGPRS.read();
      Serial.write(d);
      //Serial.println();
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
      //Serial.println();
  }
  
  cmd = "AT+CIPSTART=3,\"TCP\",\"";// Setup TCP connection
  cmd += host;
  cmd += "\",80";

sendmyGPRS(cmd);
  delay(4000);

     if( myGPRS.find( "CONNECTED" ) )
  {
      //d = myGPRS.read();
      //Serial.write(d);
      Serial.println("Connected to Thingspeak");
    //return;
  }


String cmd1 = "AT+CIPSEND=3,100";// Setup TCP connection
sendmyGPRS(cmd1);
delay(2000);

cmd = GET + "&field1="+flat+"&field2="+flon+"\r\n";

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


void gpsdump(TinyGPS &gps)
{

  //unsigned long age, date, time, chars;
  //int year;
  //byte month, day, hour, minute, second, hundredths;
  //unsigned short sentences, failed;

  gps.get_position(&lat, &lon);
  flat = lat;
  flon = lon;
  Serial.print("Lat/Long = "); 
  Serial.print(flat,6);
  Serial.print(", "); 
  Serial.println(flon,6);
  
  }
  
void sendmyGPRS(String cmd)
{
  myGPRS.println(cmd);
  //Serial.println(cmd);
}
