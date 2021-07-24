#include <SoftwareSerial.h>
SoftwareSerial mySerial(7, 8);
#include "HX711.h"

#define trigPin 12
#define echoPin 11
#define DOUT  A3
#define CLK  A2

HX711 scale(DOUT, CLK);

float calibration_factor = 182540; 
String Message;

void setup(){
  
 mySerial.begin(115200);
 Serial.begin(115200);
 pinMode(trigPin, OUTPUT);
 pinMode(echoPin, INPUT);

 scale.set_scale();
 scale.tare();  //Reset the scale to 0

 long zero_factor = scale.read_average(); //Get a baseline reading
 Serial.print("Zero factor: "); //This can be used to remove the need to tare the scale. Useful in permanent scale projects.
 Serial.println(zero_factor);
}

 void loop() {
   
 int duration, distance; // Duration used to calculate distance
 
 digitalWrite(trigPin, HIGH); 
 delay(1000); 
 digitalWrite(trigPin, LOW);
 duration = pulseIn(echoPin, HIGH);
 distance = (duration/2) * 0.0343;
 Serial.print("distance=");
 Serial.println(distance);
 
 scale.set_scale(calibration_factor); //Adjust to this calibration factor

  int binweight=scale.get_units()*453;
  
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
    
 //Delay 100ms before next reading.
 delay(100);
 }
void SendMessage(String text)
{
  mySerial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);  // Delay of 1000 milli seconds or 1 second
  mySerial.println("AT+CMGS=\"+91mobile number\"\r"); // Replace x with mobile number
  delay(1000);
  mySerial.println(text);// The SMS text you want to send
  delay(100);
  mySerial.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
}
