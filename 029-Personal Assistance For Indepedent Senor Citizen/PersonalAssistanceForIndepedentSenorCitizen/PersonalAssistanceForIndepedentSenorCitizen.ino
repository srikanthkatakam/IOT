#include <SPI.h>
#include <Ethernet.h>
#include <Keypad.h>

// Local Network Settings
byte mac[] = { 0xD4, 0x28, 0xB2, 0xFF, 0xA0, 0xA1 }; // Must be unique on local network

const byte numRows= 4; //number of rows on the keypad
const byte numCols= 4; //number of columns on the keypad
boolean DEBUG = true;
char DEVID1[] = "vF5B078D89EFD1DE";
char DEVID2[] = "v04BBFED577FC583";
char DEVID3[] = "vAEE383548E060F3";
char DEVID4[] = "v0974FE139AF7E94";

//keymap defines the key pressed according to the row and columns just as appears on the keypad
char keymap[numRows][numCols]= 
{
{'1', '2', '3'}, 
{'4', '5', '6'}, 
{'7', '8', '9'},
};

byte rowPins[numRows] = {9,8,7}; //Rows 0 to 3
byte colPins[numCols]= {5,4,3}; //Columns 0 to 3

//initializes an instance of the Keypad class
Keypad myKeypad= Keypad(makeKeymap(keymap), rowPins, colPins, numRows, numCols);                

const char* host = "api.pushingbox.com";
const int httpPort = 80;

//String PushingBoxNotifier = "GET /pushingbox?devid=v4BAEAF22D935291";                    

// Initialize Arduino Ethernet Client
EthernetClient client;  

  
void setup() {
  Serial.begin(9600); 
  delay(10);
  pinMode(2, OUTPUT);
  pinMode(6, OUTPUT);
  digitalWrite(2, LOW);
  digitalWrite(6, LOW);  
 startEthernet();
  }

void loop() {
char keypressed = myKeypad.getKey();
if (keypressed != NO_KEY)
{
Serial.println(keypressed);
  switch (keypressed){
  
 case '1':    
      Serial.println("E-mail sending to Doctor");
      sendToPushingBox(DEVID1);
      digitalWrite(2, HIGH);
      delay(2000);
      digitalWrite(2, LOW);
      break;
 case '3':    
      Serial.println("E-mail sending to Pharmacy");
      sendToPushingBox(DEVID2);
      digitalWrite(6, HIGH);
      delay(2000);
      digitalWrite(6, LOW);
      break;
 case '4':    
      Serial.println("E-mail sending to Grocery");
      sendToPushingBox(DEVID3);
      digitalWrite(6, HIGH);
      delay(2000);
      digitalWrite(6, LOW);
      break;
 case '6':    
      Serial.println("E-mail sending to Caretaker");
      sendToPushingBox(DEVID4);
      digitalWrite(2, HIGH);
      delay(2000);
      digitalWrite(2, LOW);
      break;
}
  }
}

  
void sendToPushingBox(char devid[]){
  client.stop();
  if(DEBUG){Serial.println("connecting...");}

  if (client.connect(host, 80)) {
    if(DEBUG){Serial.println("connected");}

    if(DEBUG){Serial.println("sendind request");}
    client.print("GET /pushingbox?devid=");
    client.print(devid);
    client.println(" HTTP/1.1");
    client.print("Host: ");
    client.println(host);
    client.println("User-Agent: Arduino");
    client.println();
  } 
  else {
    if(DEBUG){Serial.println("connection failed");}
  }
}

void startEthernet()
{
  
  client.stop();

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

