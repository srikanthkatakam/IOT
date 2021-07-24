#include <ESP8266WiFi.h>
int led=D2;

/* Provide Wi-Fi credentials */
const char* ssid     = "";
const char* password = "";

/* Provide TalkBack App credentials */
const char* host = "api.thingspeak.com";
const String talkBackAPIKey = "talkback API";
const String talkBackID = "talkback ID";

String talkBackCommand,tbRequest;

WiFiClient client; // Initialize a Wi-Fi client

void setup() {
  Serial.begin(115200);
  pinMode(D2,OUTPUT);
  delay(10);
  startWiFi(); // Connect to Wi-Fi network with SSID and PASSWORD
}

void loop()
{
 
Serial.print("connecting to ");
Serial.println(host);

int value=analogRead(A0);
analogWrite(D2,value);
getTalkBack(); // Get the command issued from mobile app/web app

if(talkBackCommand == "L0") analogWrite(D3,0);
if(talkBackCommand == "L20") analogWrite(D3,205);
if(talkBackCommand == "L40") analogWrite(D3,410);
if(talkBackCommand == "L60")analogWrite(D3,615);
if(talkBackCommand == "L80")analogWrite(D3,815);
if(talkBackCommand == "L100")analogWrite(D3,1023);

delay(6000);
}


void getTalkBack()
{
tbRequest="GET /talkbacks/"+ talkBackID + "/commands/execute?api_key=" + talkBackAPIKey;
Serial.println(tbRequest);

if(!client.connected())
{
  if (client.connect("api.thingspeak.com", 80))
  {
    client.println(tbRequest);
    if (client.connected())
      {
        talkBackCommand="";
        while(client.connected() && !client.available()) delay(10); //waits for data
        while (client.connected() || client.available())
            {
              talkBackCommand = client.readString();
            }
        Serial.print("Command -> ");
        Serial.println(talkBackCommand);
        Serial.println();
      }
  client.stop();
  Serial.flush();
 }
}
}
  
void startWiFi()
{
client.stop();
Serial.println();
Serial.println("Connecting to");
Serial.println(ssid);
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
