
int LDR_Pin = A0; //analog pin 0
int val =0;
int ledPin=9;

void setup()
{
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
 
}


void loop() {
  // fade in from min to max in increments of 5 points:
  val = analogRead(A0);
   if (val>=600)
   {
    digitalWrite(ledPin, LOW);
    for (int fadeValue = 0 ; fadeValue <= 255; fadeValue += 5) {
    // sets the value (range from 0 to 255):
    analogWrite(ledPin, fadeValue);
    // wait for 30 milliseconds to see the dimming effect
    delay(30);
     }
    else
    {
      digitalWrite(ledPin,HIGH);
    }
   if (val>=600)
   {
   
  // fade out from max to min in increments of 5 points:
  for (int fadeValue = 255 ; fadeValue >= 0; fadeValue -= 5) {
    // sets the value (range from 0 to 255):
    analogWrite(ledPin, fadeValue);
    // wait for 30 milliseconds to see the dimming effect
    delay(30);
    }
   }
}
