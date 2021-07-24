#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>
#define led1 3
#define led2 5

RF24 radio(9, 10);               // nRF24L01 (CE,CSN)
RF24Network network(radio);      // Include the radio in the network
const uint16_t this_node = 01;   // Address of our node in Octal format ( 04,031, etc)
const uint16_t master00 = 00;    // Address of the other node in Octal format
unsigned long tbcommand;

const unsigned long interval = 10;  //ms  // How often to send data to the other unit
unsigned long last_sent;            // When did we last send?

int ldrvalue, irvalue;
int pole1status;

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
  if( !network.available() ) { 
    Serial.println("entered automatic mode"); 
   automatic(); //if nothing comes from the network the program always works in automatic mode only
  }
    
 
 else
 {
  Serial.println("eneterd manual mode");
  manual();//when a tbcommand is selected and pole receives a tbcommand from gateway the program then  enters manual mode
 }
 
}
void manual()
{
  
  //delay(1000);
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
delay(11000);
}
void automatic()
{
  Serial.println("automatic mode entered");
  ldrvalue = analogRead(A0);
 Serial.println(ldrvalue);
  irvalue = digitalRead(2);
 Serial.println(irvalue);
  
  if (ldrvalue >= 500 ){
    analogWrite(led1, 10);
    analogWrite(led2, 10);
    pole1status = 1;  
  if (ldrvalue >= 500 && irvalue == 1){
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
    pole1status = 1;
  }
  }
  else if (irvalue == 0 || ldrvalue < 500) {
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);
    pole1status = 0;
    
  }
 network.update();
  //===== Sending =====//
  unsigned long now = millis();
  if (now - last_sent >= interval) {   // If it's time to send a data, send it!
    last_sent = now;
        
    RF24NetworkHeader header(master00);   // (Address where the data is going)
    bool ok = network.write(header, &pole1status, sizeof(pole1status)); // Send the data
    Serial.println(pole1status);
  }
}

  
