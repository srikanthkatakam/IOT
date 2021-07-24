// Different Notifications Servers 
#include <ESP8266WiFi.h>

const char* ssid     = "your SSID";
const char* password = "password";

const char* host1 = "maker.ifttt.com";
const char* host2 = "api.pushingbox.com";
const char* host3 = "smshorizon.co.in";
const String IFTTT_Event = "button"; 
const String Maker_Key = "hp6E0mOyit50FuCCCd3S2tG9tt_H2Hln2wsBe9oZZMl";

String clientResponce;



String IftttNotifier = "POST /trigger/"+IFTTT_Event+"/with/key/"+Maker_Key +" HTTP/1.1\r\n" +
                    "Host: " + host1 + "\r\n" + 
                    "Content-Type: application/x-www-form-urlencoded\r\n\r\n";
                    


String SmsNotifier = "GET /api/sendsms.php?user=sams34&apikey=tJnsxhk4icHWTWX6FkxO&mobile=mobile number&message=Temperature at Home is Very High&senderid=MYTEXT&type=txt";



String PushingBoxNotifier = "GET /pushingbox?devid=vDF2CD7A421BFC2A";



WiFiClient client;                

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid); 
  startWiFi();
  } 


void loop() {
  
float Temperature = (3.3* 100 * analogRead(A0))/1024;
Serial.println(Temperature);
delay(5000);

if (Temperature >= 38){
  TriggerIFTTT();
  delay(15000);
  TriggerPushingbox();
  delay(15000);
  TriggerSMS();
  delay(15000);
}
else {
  Serial.println("Temperature Within Limits");
}
}

void TriggerIFTTT(){
  
  Serial.print("connecting to ");
  Serial.println(host1);
  const int httpPort = 80;
  
  if (client.connect(host1, httpPort)) {
  Serial.println("Client Connected to IFTTT");
  client.println(IftttNotifier);
  }
  
  if (client.connected()){
  while(client.connected() && !client.available()) delay(10); //waits for data
  while (client.connected() || client.available()){
  char charIn = client.read();
  Serial.println(charIn);
  client.stop();
  }
  }
  client.stop();
  Serial.flush();
}

void TriggerPushingbox(){
  
  Serial.print("connecting to ");
  Serial.println(host2);
  const int httpPort = 80;
  
  if (client.connect(host2, httpPort)) {
  Serial.println("Client Connected to Pushbullet");
  client.println(PushingBoxNotifier);
  }
  
  if (client.connected()){
  while(client.connected() && !client.available()) delay(10); //waits for data
  while (client.connected() || client.available()){
  char charIn = client.read();
  Serial.print(charIn);
  client.stop();
  }
  }
  client.stop();
  Serial.flush();
}

void TriggerSMS(){
  
  Serial.print("connecting to ");
  Serial.println(host3);
  const int httpPort = 80;
  
  if (client.connect(host3, httpPort)) {
  Serial.println("Client Connected to SMS");
  client.println(SmsNotifier);
  }
  
  if (client.connected()){
  while(client.connected() && !client.available()) delay(10); //waits for data
  while (client.connected() || client.available()){
  char charIn = client.read();
  Serial.println(charIn);
  client.stop();
  }
  }
  client.stop();
  Serial.flush();
}

void startWiFi()
{
client.stop();
Serial.println();
Serial.println("Connecting Arduino to network...");
WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
delay(1000);
}
