#include "DHT.h"
#define DHTPIN 2     // what pin we're connected to
#define DHTTYPE DHT11   // define type of sensor DHT 11
DHT dht (DHTPIN, DHTTYPE);

const int Air = A1;// Analog input pins
const int Illumination = A2;
int A;
int I;


//Arrays for the 4 inputs**********************************************
int sensorValue[4] = {0, 0, 0, 0};
int voltageValue[4] = {0, 0, 0, 0};

//Char used for reading in Serial characters
char inbyte = 0;

//*******************************************************************************************

void setup() {
  // initialise serial communications at 9600
  Serial.begin(9600);
}

void loop() {
  readSensors();
  getVoltageValue();
  sendAndroidValues();
  delay(10000);
}

void readSensors()
{
  // read the analog in value to the sensor array
  sensorValue[0] =dht.readHumidity();
  sensorValue[1] = dht.readTemperature();
  A = analogRead(Air);
  sensorValue[2] = map(A,0,1023,0,100);
  I = analogRead(Illumination);
  sensorValue[3] = map(I,0,950,0,100);
}
//sends the values from the sensor over serial to BT module
void sendAndroidValues()
{
  //puts # before the values so our app knows what to do with the data
  Serial.print('#');
  //for loop cycles through 4 sensors and sends values via serial
  for (int k = 0; k < 4; k++)
  {
    Serial.print(voltageValue[k]);
    Serial.print('+');
    //technically not needed but I prefer to break up data values
    //so they are easier to see when debugging
  }
  Serial.print('~'); //used as an end of transmission character - used in app for string length
  Serial.println();
  delay(3000);        //added a delay to eliminate missed transmissions
}

void getVoltageValue()
{
  for (int x = 0; x < 4; x++)
  {
    voltageValue[x] = sensorValue[x];
  }
}
