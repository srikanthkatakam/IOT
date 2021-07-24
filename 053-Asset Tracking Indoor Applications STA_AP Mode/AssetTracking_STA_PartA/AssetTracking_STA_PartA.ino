#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>

SoftwareSerial BTSerial(D1, D2);
String data1;
long rssi;
double d;
int distance;
int incoming ;
/*****************************************************/
const char* WIFI_SSID = "ssid";
const char* WIFI_PASSWORD = "password";

/*****************************************************/

IPAddress server_ip(192,168,1,1); 
const int SERVER_PORT = 8080;

WiFiClient client;

void setup()
{
  Serial.begin(9600);
  BTSerial.begin(9600);
  pinMode(D3, OUTPUT);
  digitalWrite(D3, LOW);
  /* Setup ESP8266 in station mode */
  Serial.println();
  Serial.println("Starting up...");
  WiFi.mode(WIFI_STA); 
  WiFi.begin(WIFI_SSID,WIFI_PASSWORD);
  Serial.println();
  Serial.print("Waiting for wireless network.");
  
  while(WiFi.status() != WL_CONNECTED) 
  {
    Serial.print(".");
    delay(500);
 }
  
  Serial.println();
  Serial.println("WiFi connection established!");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP()); 
  client.connect(server_ip,SERVER_PORT);
}

void loop()
{
  rssi = WiFi.RSSI();
  Serial.print("RSSI:");
  Serial.println(rssi);
  d = pow(10, ((-65) - rssi) / (10 * 1.6));
  //Serial.print("Distance:");
  //Serial.print(d);
  //Serial.println( "m");
  senddata();
  BT_Control();     
}

void senddata(){
  
  /* Format data prior to sending */
  data1 = String (d);
 // Serial.println(data1);

   if (!client.connect(server_ip,SERVER_PORT))
  {
    Serial.println("Cilent Not Connected");
    ESP.reset();
    delay(100);
  }
    
  /* Send data to master. */
  if (client.connect(server_ip,SERVER_PORT))
  {
    Serial.println();
    Serial.println("Sending data...");
    Serial.println(data1);
    client.println(data1);
    Serial.println("Data sent.");
  }
  delay(3000);
    data1 = "";
 }

 void BT_Control(){
 while(BTSerial.available()){  //Check if there is an available byte to read
incoming = BTSerial.read(); //Read what we recevive 
    Serial.print("Received:"); 
    Serial.println(incoming);

    if (incoming == '1')
        {
     for(int i=1;i<20;i++)
      {
        
        digitalWrite(D3, HIGH);
        delay(100);
        digitalWrite(D3, LOW);
        delay(100);

      }
        }
        
    if (incoming == '0')
        {
        digitalWrite(D3, LOW);
        
        }     
  }
}
