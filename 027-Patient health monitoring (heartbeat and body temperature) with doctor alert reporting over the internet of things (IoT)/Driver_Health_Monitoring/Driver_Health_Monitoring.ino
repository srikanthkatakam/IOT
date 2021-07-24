/*
>> Pulse Sensor Amped 1.2 << >>> Pulse Sensor purple wire goes to Analog Pin 0 <<<
Pulse Sensor sample aquisition and processing happens in the background via Timer 2 interrupt. 2mS sample rate.
PWM on pins 3 and 11 will not work when using this code, because we are using Timer 2!
The following variables are automatically updated:
Signal :    int that holds the analog signal data straight from the sensor. updated every 2mS.
IBI  :      int that holds the time interval between beats. 2mS resolution.
BPM  :      int that holds the heart rate value, derived every beat, from averaging previous 10 IBI values.
QS  :       boolean that is made true whenever Pulse is found and BPM is updated. User must reset.
Pulse :     boolean that is true when a heartbeat is sensed then false in time with pin13 LED going out.
*/

#include <SoftwareSerial.h>
SoftwareSerial mySerial(7, 8);     // Connect 7 to Tx and 8 to Rx on GSM board

//  VARIABLES
int pulsePin = 1;                 // Pulse Sensor purple wire connected to analog pin 0
int tempPin = 0;                  // Temperature Sensor connected to analog pin 1
int blinkPin = 13;                // pin to blink led at each beat
int sw=4;                         // Emergency Button
int buttonState = 0;
float temp;


// these variables are volatile because they are used during the interrupt service routine!
volatile int BPM;                   // used to hold the pulse rate
volatile int Signal;                // holds the incoming raw data
volatile int IBI = 600;             // holds the time between beats, must be seeded! 
volatile boolean Pulse = false;     // true when pulse wave is high, false when it's low
volatile boolean QS = false;        // becomes true when Arduoino finds a beat.
String Message;


void setup(){
  pinMode(blinkPin,OUTPUT);         // pin that will blink to your heartbeat!
  mySerial.begin(115200);
  Serial.begin(115200);               // we agree to talk fast!
  pinMode(sw, INPUT);
  interruptSetup();                 // sets up to read Pulse Sensor signal every 2mS   
}

void loop(){
  buttonState = digitalRead(sw);
  getSensorValues();
  
  if (buttonState==HIGH){
      Message = "Iam in Emergency, Please help me";
      SendMessage(Message); 
  }

 if (BPM <=60 || BPM >=100){
      Message = "My Pulse Rate is Abnormal, Please help me";
      SendMessage(Message); 
  }
  
 if (temp <=19 || temp >=39){
      Message = "My Body Temperature is Abnormal, Please help me";
      SendMessage(Message); 
  } 
  
 // Code to print the response of GSM board
 /* if (mySerial.available()>0) {
      while (mySerial.available() !=0) {
      char c = mySerial.read();
      delay(50);
      Serial.write(c);
      delay(50);
      }
  } */
  
 delay(1000);
}

void getSensorValues()
{
  // read the analog in value to the sensor array
 temp = ( 5 * analogRead(tempPin) * 100.0) / 1024.0;
 Serial.print("Body Temperature=");
 Serial.println(temp);
 
 if (QS == true){                       // Quantified Self flag is true when arduino finds a heartbeat
      //Serial.print("Signal=");
      //Serial.println(Signal);           // Print pulse sensor raw data
      Serial.print("HeartBeat=");
      Serial.println(BPM);              // Print BPM (beats per Minut)
      //Serial.print("Time Interval=");
      //Serial.println(IBI);              // Print IBI (Interval between beats)
      QS = false;                       // reset the Quantified Self flag for next time    
     }
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
