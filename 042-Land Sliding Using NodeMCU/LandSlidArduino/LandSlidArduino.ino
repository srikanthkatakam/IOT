 #include <ArduinoJson.h>

#include "XBee.h"
#include <SoftwareSerial.h>
int refreshRate = 3000; // in MS, the amount of time between iterations
//String deviceID = "Node2"; // the name of the device, for use in the JSON object that is published in the Zigbee payload
uint8_t recv = 10; 
uint8_t trans = 11; 
SoftwareSerial soft_serial(recv, trans);

XBee xbee = XBee();
ZBRxIoSampleResponse ioSample = ZBRxIoSampleResponse();

// Specify the address of the remote XBee (this is the SH + SL)
XBeeAddress64 addr64 = XBeeAddress64(0x13A200, 0x415ABE6F);
//const int Flux =  A0;
//const int  Vibration =  A1;
//const int Force =  A2;

int Flux , Vibration, Force;

void setup() 
{
 // analogReference(EXTERNAL);
  Serial.begin(9600);
  soft_serial.begin(9600);
  xbee.setSerial(soft_serial);
}
void loop() {
  Flux = analogRead(A0);
    Vibration = analogRead(A3);
      Force = analogRead(A2);
    
delay(1000);

      String payloadStr = buildJSON();
      //Serial.println("hello");
      publishZWaveTx(payloadStr);
      
     Serial.println(payloadStr);
 
}

void publishZWaveTx(String payloadStr) {
  char payload[payloadStr.length() + 1];
  payloadStr.toCharArray(payload, payloadStr.length() + 1);   

  ZBTxRequest zbTx = ZBTxRequest(addr64, (uint8_t *) payload, strlen(payload));
delay(4000);
  // Send your request
  xbee.send(zbTx);
}
String buildJSON() {
  String payloadStr;

  payloadStr = "{";
  //payloadStr += "\"id\":\"" + deviceID + "\",";
   payloadStr += "\"Flux\":\"" +String (Flux,DEC)+ "\",";
     payloadStr += "\"Vibration\":\"" +String (Vibration,DEC)+ "\",";
       payloadStr += "\"Force\":\"" +String (Force,DEC)+ "\""; payloadStr += "}";  

  return (payloadStr);
}
