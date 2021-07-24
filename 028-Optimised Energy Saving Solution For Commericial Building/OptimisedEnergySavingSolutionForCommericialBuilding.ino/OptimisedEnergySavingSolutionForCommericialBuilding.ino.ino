//Optimised Energy Saving Solution For Commerical Building

#include <Ethernet.h>
#include <SPI.h>
#include <DHT.h>
#define DHTPIN 2     // what pin we're connected to
#define DHTTYPE DHT11   // define type of sensor DHT 11 

DHT dht (DHTPIN, DHTTYPE);

#include <UbidotsEthernet.h>
#define ID  "576e36837625427413b0f1ea"  // LDR Value
#define ID2 "576e369f7625427562674f26" // Current Value
#define ID3 "576e366f76254272f258782a" //Voltage Value
#define ID4 "576cc72a7625422519b2e630" //PIR Value
#define ID5 "576cc72a7625422519b2e630" //Humidity Value
#define ID6 "576cc72a7625422519b2e630" //Temperature Value
#define TOKEN  "2xy8eyjf5xpSJzdwrjbCbAGWoiqUje"  // Put here your Ubidots TOKEN

// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
// Set the static IP address to use if the DHCP fails to assign

Ubidots client(TOKEN);

void setup(){
  pinMode(3, INPUT);
    Serial.begin(9600);
    dht.begin();
    startEthernet();
    
}
void loop(){
    delay(2000);
    float h = dht.readHumidity();
    float t = dht.readTemperature();

    float Current = (analogRead((A0)-511)*5)/(0.1*1024);
    float V =  (analogRead(A1),DEC);
    float Voltage = map(V,0,1023,0,230);
    float LDR = analogRead(A2);
    float PIR = digitalRead(3);
    client.add(ID, LDR);
    client.add(ID2, Current);
    client.add(ID3, Voltage);
    client.add(ID4, PIR);
    client.add(ID5, h);
    client.add(ID6, t);
    client.sendAll();
}
void startEthernet()
{
  
  //client.stop();

  // Connect to network amd obtain an IP address using DHCP
  if (Ethernet.begin(mac) == 0)
  {
    Serial.println("DHCP Failed, reset Arduino to try again");
    Serial.println();
  }
  else
  {
    Serial.println("Arduino connected to network using DHCP");
    Serial.println(Ethernet.localIP());
    Serial.println();
  }
  
  delay(1000);
}
