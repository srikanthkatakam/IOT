#include <SoftwareSerial.h>
#include <XBee.h>
#include <SPI.h>
#include <RFID.h>
   
#define SS_PIN 10
#define RST_PIN 9

RFID rfid(SS_PIN,RST_PIN);
 
int serNum[5];
int Card1_Status = 0 ;
  
String cancat_card = " ";

/// //////////////////@  router
String deviceID = "RFIDNode1"; // the name of the device, for use in the JSON object that is published in the Zigbee payload
uint8_t recv = 7; 
uint8_t trans = 8; 
SoftwareSerial soft_serial(recv, trans);

XBee xbee = XBee(); 
ZBRxIoSampleResponse ioSample = ZBRxIoSampleResponse();
// Specify the address of the remote XBee (this is the SH + SL)
XBeeAddress64 addr64 = XBeeAddress64(0x13A200,0x415138F2);

void setup() 
{
  Serial.begin(9600);
  soft_serial.begin(9600);
  xbee.setSerial(soft_serial);
  delay(10);
  SPI.begin();
  rfid.init();
  
}

void loop() {
    if(rfid.isCard()){
                        if(rfid.readCardSerial()){
                                      // Serial.print(rfid.serNum[0]);
                                      // Serial.print(" ");
                                      // Serial.print(rfid.serNum[1]);
                                      // Serial.print(" ");            
                                      // Serial.print(rfid.serNum[2]);             
                                      // Serial.print(" ");
                                      // Serial.print(rfid.serNum[3]);
                                      // Serial.print(" ");
                                      // Serial.print(rfid.serNum[4]);
                                      // Serial.println("");
                                       
                                        // Cancat String
                                      cancat_card = String(rfid.serNum[0])+","+String(rfid.serNum[1])+","+String(rfid.serNum[2])+","+String(rfid.serNum[3])+","+String(rfid.serNum[4]);
                                      
          }
       }
    delay(1000);
    rfid.halt();

///////////////////
      String payloadStr = buildJSON();
      publishZWaveTx(payloadStr);
     Serial.println(payloadStr);
}

void publishZWaveTx(String payloadStr) {
  char payload[payloadStr.length() + 1];
  payloadStr.toCharArray(payload, payloadStr.length() + 1);   

  ZBTxRequest zbTx = ZBTxRequest(addr64, (uint8_t *) payload, strlen(payload));
  // Send your request
  xbee.send(zbTx);
cancat_card = "";
}

String buildJSON() {
  String payloadStr;
  payloadStr += "{";
  payloadStr += "\"id\":\"" + deviceID + "\",";
  payloadStr += "\"RFID\":\"" +String (cancat_card)+ "\""; 
  payloadStr += "}";  

  delay(3000);
  return (payloadStr);
  
}
