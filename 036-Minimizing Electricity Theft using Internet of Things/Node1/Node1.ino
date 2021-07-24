#include "XBee.h"
#include <SoftwareSerial.h>

String deviceID = "Node1"; // the name of the device, for use in the JSON object that is published in the Zigbee payload
uint8_t recv = 10; 
uint8_t trans = 11; 
SoftwareSerial soft_serial(recv, trans);

XBee xbee = XBee(); 
ZBRxIoSampleResponse ioSample = ZBRxIoSampleResponse();

// Specify the address of the remote XBee (this is the SH + SL)
XBeeAddress64 addr64 = XBeeAddress64(0x13A200, 0x41628C1D);
const int xInput =  A0;

int val1 , val2;

void setup() 
{
 // analogReference(EXTERNAL);
  Serial.begin(9600);
  soft_serial.begin(9600);
  xbee.setSerial(soft_serial);
}
void loop() {
  val1 = analogRead(A0);
  Serial.print("val1:");
  Serial.println(val1);
  //val2= map(val1, 511, 540,0.1, 5.0);
    val2 = ((val1-511)*5)/(0.1*1023);
    
    Serial.print("val2:");
Serial.println(val2);
      
      String payloadStr = buildJSON();
      //Serial.println("hello");
      publishZWaveTx(payloadStr);
      
     Serial.println(payloadStr);

}

void publishZWaveTx(String payloadStr) {
  char payload[payloadStr.length() + 1];
  payloadStr.toCharArray(payload, payloadStr.length() + 1);   

  ZBTxRequest zbTx = ZBTxRequest(addr64, (uint8_t *) payload, strlen(payload));
delay(2000);
  // Send your request
  xbee.send(zbTx);
}
String buildJSON() {
  String payloadStr;

  payloadStr = "{";
  payloadStr += "\"id\":\"" + deviceID + "\",";
  
       payloadStr += "\"value\":\"" +String (val2)+ "\""; payloadStr += "}";  

  return (payloadStr);
}
