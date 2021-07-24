#include <TinyGPS++.h>   
#include "HX711.h"

TinyGPSPlus gps;


#define trigPin 12
#define echoPin 11
#define DOUT  A3
#define CLK  A2

HX711 scale(DOUT, CLK);

float calibration_factor = 182540; 
String Message;
int d;
 int duration, distance, binweight;

int  MQ7map;
float flat,flon;
String lat, lon, flag;
unsigned long age;
bool newData = false;


#include <SoftwareSerial.h>

SoftwareSerial mySerial(9, 10 ); //RX, TX
SoftwareSerial myGps(3, 4); //RX, TX
void setup() {
Serial.begin(9600);
mySerial.begin(9600);
myGps.begin(9600);
 pinMode(trigPin, OUTPUT);
 pinMode(echoPin, INPUT);

 scale.set_scale();
 scale.tare();  //Reset the scale to 0

 long zero_factor = scale.read_average(); //Get a baseline reading
 Serial.print("Zero factor: "); //This can be used to remove the need to tare the scale. Useful in permanent scale projects.
 Serial.println(zero_factor);


}

void loop() {
  values();
  delay(1000);
  getGPSdata();
  delay(1000);
  sendcmd();
  delay(1000);
}

void values(){

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

int MQ7=analogRead(A2);
MQ7map=map(MQ7,0,1023,0,100);
Serial.print("MQ7 Value is: ");
Serial.println(MQ7map);

}

void getGPSdata(){
 
  flat = gps.location.lat();
  lat = String(flat,6);
  Serial.print("Latitude  : ");
  Serial.println(lat);
  flon = gps.location.lng();
  lon = String(flon,6);
  Serial.print("Latitude  : ");
  Serial.println(lon);
  smartDelay(1000);
  
  
 
  

  if (millis() > 5000 && gps.charsProcessed() < 10)
    Serial.println(F("No GPS data received: check wiring"));
} 
static void smartDelay(unsigned long ms)                // This custom version of delay() ensures that the gps object is being "fed".
{
  unsigned long start = millis();
  do 
  {
    while (myGps.available())
      gps.encode(myGps.read());
  } while (millis() - start < ms);
}

void sendcmd(){

String cmd = "#&field1="+String(lat)+"&field2="+String(lon)+"&field3="+String(distance)+"&field4="+String(binweight)+"&field5="+String(MQ7map);
Serial.println(cmd);
Serial.println(cmd.length());

mySerial.println(cmd);

}
