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
SoftwareSerial mySerial(8,9);

//  VARIABLES
int pulsePin = 1;                 // Pulse Sensor purple wire connected to analog pin 0
int tempPin = 0;                  // Temperature Sensor connected to analog pin 1
int blinkPin = 13;                // pin to blink led at each beat
int sensorValue[2] = {0, 0};
int voltageValue[2] = {0, 0};
char inbyte = 0;


// these variables are volatile because they are used during the interrupt service routine!
volatile int BPM;                   // used to hold the pulse rate
volatile int Signal;                // holds the incoming raw data
volatile int IBI = 600;             // holds the time between beats, must be seeded! 
volatile boolean Pulse = false;     // true when pulse wave is high, false when it's low
volatile boolean QS = false;        // becomes true when Arduoino finds a beat.


void setup(){
  pinMode(blinkPin,OUTPUT);         // pin that will blink to your heartbeat!
  Serial.begin(9600);               // we agree to talk fast!
  mySerial.begin(9600);             // 
  interruptSetup();                 // sets up to read Pulse Sensor signal every 2mS 
   
   // UN-COMMENT THE NEXT LINE IF YOU ARE POWERING The Pulse Sensor AT LOW VOLTAGE, 
   // AND APPLY THAT VOLTAGE TO THE A-REF PIN
   //analogReference(EXTERNAL);
      
}

void loop(){
  getSensorValues();
  getVoltageValue();
  sendAndroidValues();
  delay(20);                      //  take a break
}


void getSensorValues()
{
  // read the analog in value to the sensor array
 sensorValue[0] = ( 5 * analogRead(tempPin) * 100.0) / 1024.0;
 
 if (QS == true){                       // Quantified Self flag is true when arduino finds a heartbeat
      Serial.print("Signal=");
      Serial.println(Signal);           // Print pulse sensor raw data
      Serial.print("HeartBeat=");
      Serial.println(BPM);              // Print BPM (beats per Minut)
      Serial.print("Time Interval=");
      Serial.println(IBI);              // Print IBI (Interval between beats)
      sensorValue[1] = BPM;             // read the BPM to the sensor array 
      QS = false;                       // reset the Quantified Self flag for next time    
     }
}

void sendAndroidValues()
{
  //puts # before the values so our app knows what to do with the data
  mySerial.print('#');
  Serial.print('#');
  //for loop cycles through 2 sensors and sends values via serial
  for (int k = 0; k < 2; k++)
  {
    mySerial.print(voltageValue[k]);
    Serial.print(voltageValue[k]);
    mySerial.print('+');
    Serial.print('+');
    //technically not needed but I prefer to break up data values
    //so they are easier to see when debugging
  }
  mySerial.println('~'); //used as an end of transmission character - used in app for string length
  Serial.println('~');
  delay(2000);        //added a delay to eliminate missed transmissions
}

void getVoltageValue()
{
  if (BPM>=60 && BPM<=100){
  for (int x = 0; x < 2; x++)
  {
    voltageValue[x] = sensorValue[x];
  }
  }
  else {
    voltageValue[1] = 0;
  }
}





