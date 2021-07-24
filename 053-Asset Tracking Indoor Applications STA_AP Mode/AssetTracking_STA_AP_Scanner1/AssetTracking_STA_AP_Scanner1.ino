#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>

const char*  MySSID     = "ssid"; // Local Network SSID
const char*  MyPASSWORD = "password"; // Local Network Password
const String PART_ID = "1";

/*****************************************************/
const char*  WIFI_SSID     = "MyWiFi";
const char*  WIFI_PASSWORD = "MyPassword";
/*****************************************************/

const char* host = "api.thingspeak.com";
const char* privateKey = "thingspeak write key";

const int WIFI_CHANNEL  = 4;
const int SERVER_PORT = 8080;

IPAddress local_IP(192,168,1,1);
IPAddress gateway(192,168,0,1);
IPAddress subnet(255,255,255,0) ;

WiFiServer WiFi_Server(SERVER_PORT);
WiFiClient WiFi_Client;
//WiFiClient client;

String data1, data2;
float c;
String Lat="-33.772793";
String Long="150.908142";
void setup()
{
  Serial.begin(115200);
  Serial.println();
  /* Setup ESP8266 in soft-AP mode*/
  WiFi.mode(WIFI_AP_STA);
  Serial.println();
  Serial.print("Setting soft-AP configuration ... ");
  Serial.println(WiFi.softAPConfig(local_IP, gateway, subnet) ? "Ready" : "Failed!");
  Serial.print("Setting soft-AP ... ");
  Serial.println(WiFi.softAP(WIFI_SSID,WIFI_PASSWORD,WIFI_CHANNEL) ? "Ready" : "Failed!");
  Serial.print("Soft-AP IP address = ");
  Serial.println(WiFi.softAPIP());

  /* Starting the Server */
  WiFi_Server.begin();
  Serial.println("Server online.");
  delay(1000);
  Serial.println();
 
  //Station
 // WiFi.hostname("ESP8266");
  WiFi.begin(MySSID, MyPASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500); 
  }
  
  Serial.println("WiFi connected");  
  Serial.println("Local IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  
}

void loop()
{
  WiFi_Client = WiFi_Server.available();
 
  if (WiFi_Client.available() > 0)
  {
      data1 =WiFi_Client.readStringUntil('\r');
      Serial.println(data1);
      c = data1.toFloat(); 
 
   if (c >= 0.01){
    Serial.print("connecting to ");
  Serial.println(host);
  
  // Use WiFiClient class to create TCP connections
  
  const int httpPort = 80;
  if (!WiFi_Client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
   
  // We now create a URI for the request
  String url = "/update";
  url += "?api_key=";
  url += privateKey;
  url += "&field1=";
  url += PART_ID;
  url += "&field2=";
  url += data1;
  url += "&field3=";
  url += Lat;
  url += "&field4=";
  url += Long;
  
  
  Serial.print("Requesting URL: ");
  Serial.println(url);
  
  // This will send the request to the server
  WiFi_Client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  
  
  // Read all the lines of the reply from server and print them to Serial
  while(WiFi_Client.available()){
    String line = WiFi_Client.readStringUntil('\r');
    Serial.print(line);
  }
  
  Serial.println();
  Serial.println("closing connection");
  delay(1000);
  }
 }
   else{
    Serial.println("No Data Received From Tag");
    delay(1000);
   }
   data1 = "";
}
