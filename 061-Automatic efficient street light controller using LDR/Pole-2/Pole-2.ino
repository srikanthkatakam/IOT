#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>
#define led1 3
#define led2 5

RF24 radio(9, 10);               // nRF24L01 (CE,CSN)
RF24Network network(radio);      // Include the radio in the network
const uint16_t this_node = 02;   // Address of our node in Octal format ( 04,031, etc)
const uint16_t master00 = 00;    // Address of the other node in Octal format
unsigned long tbcommand;

void setup() {
  Serial.begin(115200);
  SPI.begin();
  radio.begin();
  network.begin(90, this_node); //(channel, node address)
  radio.setDataRate(RF24_2MBPS);
  pinMode(led1,OUTPUT);
  pinMode(led2,OUTPUT);
  analogWrite(led1,0);
  analogWrite(led2,0);
}
void loop() {
  network.update();
  //===== Receiving =====//
  while ( network.available() ) {     // Is there any incoming data?
    RF24NetworkHeader header;
    
    network.read(header, &tbcommand, sizeof(tbcommand)); // Read the incoming data
    Serial.println(tbcommand);
  
if(tbcommand == 1)
{
  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
 
}  
if(tbcommand== 50){
  analogWrite(led1, 10);
  analogWrite(led2, 10);
  
  }
if(tbcommand== 100){
  digitalWrite(led1, HIGH);
  digitalWrite(led2, HIGH);

  }
  }
  
}
