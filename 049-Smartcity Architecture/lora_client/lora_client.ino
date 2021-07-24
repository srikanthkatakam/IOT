
#include <SPI.h>
#include <RH_RF95.h>
RH_RF95 rf95;
int aq ,co ,airquality,carbonmonooxide;
String totaldata;

String datastring="",deviceId = "1";
char databuf[50];
char databuf1[50];
uint8_t dataoutgoing[10];
uint8_t dataoutgoing1[10];

void setup() 
{
  
  Serial.begin(9600);
  while (!Serial) ; // Wait for serial port to be available
  if (!rf95.init())
    Serial.println("init failed");
  
}

void loop()
{
  Serial.println("Sending to rf95_server");
   aq =analogRead(A0);//Reading voltage from A0 of Arduino
   co =analogRead(A1);//Reading current from A1 of Arduino
   airquality = map(aq,0,1023,0,100);
   carbonmonooxide = map(co,0,1023,0,100);
   
  
  Serial.print("quality of air is ");Serial.println(airquality);
  Serial.print("The value of carbonmonooxide is:");Serial.println(carbonmonooxide);
  

  String totaldata= "@"+String(deviceId)+","+String(airquality)+","+String(carbonmonooxide)+"@";

   
  Serial.print("The sensor values to be sent to server are:");
  Serial.println(totaldata);


   for(int i=0;i<(totaldata.length());i++){
    databuf[i]=totaldata[i];
} 
delay(1000); 
 strcpy((char *)dataoutgoing,databuf); 
  Serial.print("The data in databuf is:") ;
  Serial.println(databuf);
  //delay(4000) ;
 rf95.send(dataoutgoing, sizeof(dataoutgoing));
  //rf95.send(dataoutgoing);
 
   
  rf95.waitPacketSent();

 
  delay(1000);
  
}



