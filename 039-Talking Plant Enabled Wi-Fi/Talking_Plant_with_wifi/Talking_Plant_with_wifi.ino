// LIBRARY DECLARATION
#include <ESP8266WiFi.h>
#include <DHT.h>
#include <Servo.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


#define DHTPIN D0  
#define DHTTYPE DHT11  
DHT dht (DHTPIN, DHTTYPE);

int pirPin=D3; //the digital pin connected to the pirsensor's output

// OLED DECLAsRATION
#define OLED_SDA   D7  //MOSI - D1 pin OLED display
#define OLED_SCL   D5  //CLK  - D0 pin OLED display
#define OLED_DC    D2  //
#define OLED_CS    D8  // no need of connecting, just use some pin number
#define OLED_RESET D1  //RES

Adafruit_SSD1306 display(OLED_SDA,OLED_SCL, OLED_DC, OLED_RESET, OLED_CS);

Servo myservo;// NAMING THE SERVO
WiFiClient client; 

const char* ssid     = "";
const char* password = "";

const char* host = "";
const char* writeAPIKey = "";

String talkBackCommand;
float t, h, moisture;


void setup() 
{

    Serial.begin(115200);
    display.begin(SSD1306_SWITCHCAPVCC);
    display.display();// internally, this will display the splashscreen.
    delay(10);  
    display.clearDisplay();// Clear the buffer.
    dht.begin();
   pinMode(pirPin,INPUT);
   
    myservo.attach(D4);
    delay(10);
    Serial.print("Connecting to ");
    Serial.println(ssid);
  
     WiFi.begin(ssid, password);
  
        while (WiFi.status() != WL_CONNECTED)
          {
            delay(500);
    Serial.print(".");
          }

    Serial.println("");
    Serial.println("WiFi connected");  
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
 
}
 
void loop() 
{
  h = dht.readHumidity();
  t = dht.readTemperature();
  moisture = analogRead(A0);
  Serial.print("temperature value is :");
  Serial.println(t);
  Serial.print("humidity value is :");
  Serial.println(h);
  delay(1000);
  Serial.println("moisture is :");
  Serial.println(moisture);
  delay(1000);
  
  if(digitalRead(pirPin)==HIGH)
  {
  Serial.println("motion detected");
  delay(1000);
  Serial.println("displayed");
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.setCursor(15,10);
  display.print("WELCOME");
  
  display.display();
  display.clearDisplay();
  }
  else
  {
  Serial.println("motion end");
  Serial.println("displayed");
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.setCursor(15,10);
  display.print("BYE....");
  display.display();
  display.clearDisplay();
  }
  if(moisture < 500 && t < 30)
     {
        Serial.println("displayed");
        display.setTextColor(WHITE); 
        display.setTextSize(2);
        display.setCursor(15,10);
        display.print("HAPPY:D" );
        display.display();
        display.clearDisplay();
 
     }

  else 
     {
        Serial.println("displayed");
        display.setTextColor(WHITE); 
        display.setTextSize(2);
        display.setCursor(15,10);
        display.print("SAD:D");
        display.display();
        display.clearDisplay();
     }
 
 getTalkBack();  
delay(1000);
upload();
delay(1000);

}

void upload()
{
Serial.print("connecting to ");
  Serial.println(host);
  
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  
  // We now create a URI for the request
  String url = "";
  url += "?api_key=";
  url += writeAPIKey;
  url += "&field1=";
  url += t;
  url += "&field2=";
  url += h;
  url += "&field3=";
  url += moisture;
 
  
  Serial.print("Requesting URL: ");
  Serial.println(url);
  
  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  delay(10);
  
  // Read all the lines of the reply from server and print them to Serial
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
  
  Serial.println();
  Serial.println("closing connection");
}

  
void getTalkBack()
{
  Serial.println("start talkback");
  String  tsData="/API/command";
  if (client.connect("smartenvironment.thesmartbridge.com", 80)) 
  {
    Serial.println("connected");
    client.print("GET "+tsData+" HTTP/1.1\n");
    client.print("Host: smartenvironment.thesmartbridge.com\n");
    client.print("Connection: close\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(tsData.length());
    client.print("\n\n");
    client.print(tsData);
   if(client.find("@@"))
    {
      String talkBackCommand = client.readStringUntil('\r');
      Serial.println(talkBackCommand);
      if(talkBackCommand == "MOTOR ON")  
      {
        Serial.println("servo is on");
        myservo.write(180);
   
      }
      else if(talkBackCommand == "MOTOR OFF")
      {
        Serial.println("servo is off");
        myservo.write(0);
      }
      else
      {
        Serial.println("displayed");
        display.setTextColor(WHITE); 
        display.setTextSize(2);
        display.setCursor(10,10);
        display.print(talkBackCommand);
        display.display();
        display.startscrollleft(0x00, 0x0F);
        delay(5000);
       // display.stopscroll();
        display.clearDisplay();
      }
  
  delay(100);
}
}
while(client.connected() ||client.available()>0 )
{
delay(5000);
String talkBackCommand = client.readString();

}

client.stop();
Serial.flush();
}





  
