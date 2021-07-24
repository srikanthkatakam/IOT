#include <Ethernet.h>
#include <SPI.h>
#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE); 

int temperature;
int humidity;
int illumination;

const String formkey = "1VUP2pFvFs6kKgLB1PQXpckbaRijRBB5yeGsVykUByY4"; //Replace with your Key
byte mac[] = { 0x90,0xA2,0xDA,0x00,0x55,0x8D};  //Replace with your Ethernet shield MAC
char server[] = "api.pushingbox.com";
String ServerResponce;
char devid[] = "v047BC94BB24F901";
char postmsg[100];

EthernetClient client;
 
void setup()
{
  Serial.begin(9600);
  startEthernet();
  delay(1000);
}
 

void loop(){
  
  temperature = dht.readTemperature();
  humidity = dht.readHumidity();
  illumination = analogRead(A0);
  
if (client.connect(server, 80)) {
    
    Serial.println("connected");
    sprintf(postmsg,"GET /pushingbox?devid=v047BC94BB24F901&status1=%d&status2=%d&status3=%d HTTP/1.1",temperature,humidity,illumination);
    Serial.println(postmsg);
    client.println(postmsg);
    client.println("Host: api.pushingbox.com");
    client.println("Connection: close");
    client.println();
     
if (client.connected()){
Serial.println("Server Response");
Serial.println();
while(client.connected() && !client.available()) delay(1); //waits for data
while (client.connected() || client.available())
{
char charIn = client.read();
ServerResponce += charIn;
}
Serial.println();
Serial.println(ServerResponce);
Serial.println();
delay(1000);
client.stop();
}  

else {
    Serial.println();
    Serial.println("disconnecting.");
    client.stop();
  }
 
delay(3000);
 
}
}




void startEthernet()
{
  client.stop();
  Serial.println("Connecting Arduino to network...");
  Serial.println();  
  delay(1000);
    
    // Connect to network amd obtain an IP address using DHCP
  if (Ethernet.begin(mac) == 0){
    Serial.println("DHCP Failed, reset Arduino to try again");
    Serial.println();
    }
  
  else{
    Serial.println("Arduino connected to network using DHCP");
    Serial.println(Ethernet.localIP());
    Serial.println();
  }
  delay(3000);
}
