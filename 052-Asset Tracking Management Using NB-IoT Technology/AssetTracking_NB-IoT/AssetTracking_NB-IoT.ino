#include <Adafruit_FONA.h>
#include <SoftwareSerial.h>
//#include <LowPower.h>
#define FONA_PWRKEY 6
#define FONA_RST 9
#define FONA_TX 7
#define FONA_RX 8 


SoftwareSerial fonaSS (FONA_RX, FONA_TX);
SoftwareSerial *fonaSerial = &fonaSS;

Adafruit_FONA_LTE fona = Adafruit_FONA_LTE();

uint8_t counter = 0;
char URL[150]; // Make sure this is long enough for your request URL
//char body[100]; // Only need this is you're doing an HTTP POST request

const int wakeUpPin = 2; // Use pin 2 to wake up the Uno/Mega
float latitude, longitude, speed_kph, heading, altitude, second;
uint8_t type;
uint16_t year;
uint8_t month, day, hour, minute;
char latBuff[50], longBuff[50], altBuff[50];
float lastlatitude, lastlongitude;
const unsigned long SECOND = 1000;
const unsigned long HOUR = 3600*SECOND;

void setup() {
  Serial.begin(115200);
  Serial.println(F("*** Burgalert 7000 ***"));

  pinMode(wakeUpPin, INPUT_PULLUP); // For the interrupt wake-up to work
 pinMode(FONA_RST, OUTPUT);
  digitalWrite(FONA_RST, HIGH); // Default state
  pinMode(FONA_PWRKEY, OUTPUT);

  powerOn(true); // Power on the module
  moduleSetup(); // Establish first-time serial comm and print IMEI
  fona.setNetworkSettings(F("telstra.iot")); 
  fona.setPreferredLTEMode(2); // Use LTE NB-IoT only 
  detachInterrupt(digitalPinToInterrupt(wakeUpPin));
  delay(1000);
}

void loop() {
  
  
  while (!netStatus()) {
    Serial.println(F("Failed to connect to cell network, retrying..."));
    delay(2000); // Retry every 2s
  }
  Serial.println(F("Connected to cell network!"));

    while (!fona.enableGPS(true)) {
    Serial.println(F("Failed to turn on GPS, retrying..."));
    delay(2000); // Retry every 2s
  }
  Serial.println(F("Turned on GPS!"));
  if (! fona.getGPS(&latitude, &longitude, &speed_kph, &heading, &altitude, &year, &month, &day, &hour, &minute, &second)) { // Use this line instead if you don't want UTC time
  Serial.println(F("Error!"));
}

  dtostrf (latitude, 1, 6, latBuff);
  dtostrf (longitude, 1, 6, longBuff); 
  dtostrf (altitude, 1, 0, altBuff);

  // Disable data connection before attempting to enable
  if (!fona.enableGPRS(false)) {
    Serial.println(F("Failed to turn off"));
  }

  // Turn on data connection after connecting to network
  while (!fona.enableGPRS(true)) {
    Serial.println(F("Failed to enable GPRS, retrying..."));
    delay(2000); // Retry every 2s
  }
  Serial.println(F("Enabled GPRS!"));
  
  counter = 0; // This counts the number of failed attempts tries

  if( lastlatitude == latitude && lastlongitude == longitude){
    
  delay(4*HOUR);
  sprintf(URL, "api.thingspeak.com/update?api_key=JRZN84F4P3CDGI91&field1=%s&field2=%s&field3=%s", latBuff, longBuff, altBuff); 
  }
  
  if( lastlatitude != latitude && lastlongitude != longitude){
  delay(60*SECOND);
  sprintf(URL, "api.thingspeak.com/update?api_key=JRZN84F4P3CDGI91&field1=%s&field2=%s&field3=%s", latBuff, longBuff, altBuff); 
  }
  
  while (counter < 3 && !fona.postData("GET", URL)) {
    Serial.println(F("Failed to post data, retrying..."));
    counter++; // Increment counter
    delay(1000);
  
  }

  // Or do an HTTP POST request instead!
  /*
  sprintf(URL, "http://dweet.io/dweet/for/%s", imei);
  body = "{burglar: true}"; // We're keeping it super simple for now!
//  sprintf(body, "{\"burglar\":true,\"temperature\":%s, tempBuff); // HTTP POST JSON body, feel free to add stuff!
  
  while (counter < 3 && !fona.postData("POST", URL, body)) {
    Serial.println(F("Failed to complete HTTP POST..."));
    counter++;
    delay(1000);
  }
  */
  
  //sleepDevice(); // Put the MCU and SIM7000 shield to sleep until more motion is detected!
  lastlatitude = latitude;
  lastlongitude = longitude;
}

// Power on/off the module
void powerOn(bool state) {
  if (state) {
    Serial.println(F("Turning on SIM7000..."));
    digitalWrite(FONA_PWRKEY, LOW);
    delay(100); // Turn on module
    digitalWrite(FONA_PWRKEY, HIGH);
    delay(5000); // Give enough time for the module to boot up before communicating with it
  }
  else {
    Serial.println(F("Turning off SIM7000..."));
   // fona.powerDown(); // Turn off module
  }
}

void moduleSetup() {
  // Note: The SIM7000A baud rate seems to reset after being power cycled (SIMCom firmware thing)
  // SIM7000 takes about 3s to turn on but SIM7500 takes about 15s
  // Press reset button if the module is still turning on and the board doesn't find it.
  // When the module is on it should communicate right after pressing reset
  fonaSS.begin(115200); // Default SIM7000 shield baud rate
  
  Serial.println(F("Configuring to 19200 baud"));
  fonaSS.println("AT+IPR=19200"); // Set baud rate
  fonaSS.begin(19200);
  if (! fona.begin(fonaSS)) {
    Serial.println(F("Couldn't find FONA"));
    while(1); // Don't proceed if it couldn't find the device
  }
  
  type = fona.type();
  Serial.println(F("FONA is OK"));
  Serial.print(F("Found "));
  switch (type) {
    case SIM7000A:
      Serial.println(F("SIM7000A (American)")); break;
    case SIM7000C:
      Serial.println(F("SIM7000C (Chinese)")); break;
    case SIM7000E:
      Serial.println(F("SIM7000E (European)")); break;
    case SIM7000G:
      Serial.println(F("SIM7000G (Global)")); break;
    default:
      Serial.println(F("???")); break;
  }
 
}

bool netStatus() {
  int n = fona.getNetworkStatus();
  
  Serial.print(F("Network status ")); Serial.print(n); Serial.print(F(": "));
  if (n == 0) Serial.println(F("Not registered"));
  if (n == 1) Serial.println(F("Registered (home)"));
  if (n == 2) Serial.println(F("Not registered (searching)"));
  if (n == 3) Serial.println(F("Denied"));
  if (n == 4) Serial.println(F("Unknown"));
  if (n == 5) Serial.println(F("Registered roaming"));

  if (!(n == 1 || n == 5)) return false;
  else return true;
}
/*
void sleepDevice() {
  Serial.println(F("Turning off SIM7000..."));
  powerOn(false); // Turn off the SIM7000 module

  // After everything's said and done, attach the interrupt
  // Set the PIR sensor jumper to "L" for normal operation. This
  // means the output will go from LOW to HIGH when motion is detected!
  attachInterrupt(digitalPinToInterrupt(wakeUpPin), wakeUp, RISING); // When the PIR sensor pin goes low to high, MCU wakes up!

  Serial.println(F("Sleeping MCU..."));
  delay(100); // Needed for the serial print above

  // Power down MCU with ADC and BOD module disabled
  // Wake up when wake up pin is low
  LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);
}*/

void wakeUp() {
  // Just a handler for the interrupt
  // This runs right after the MCU wakes up, then it goes back to loop()
}
