import time
from flask import Flask, render_template, request
app = Flask(__name__)
import pigpio
pi = pigpio.pi()


# Create a dictionary called pins to store the pin number, name, and pin state:
pins = {
   2 : {'name' : 'GPIO 2', 'state' : pigpio.PUD_DOWN}
   }

# Set each pin as an output and make it low:
for pin in pins:
   pi.set_mode(pin, pigpio.OUTPUT)

@app.route("/")
def main():
   # For each pin, read the pin state and store it in the pins dictionary:
   for pin in pins:
      pins[pin]['state'] = pi.read(pin)
   # Put the pin dictionary into the template data dictionary:
   templateData = {
      'pins' : pins
      }
   # Pass the template data into the template main.html and return it to the user
   return render_template('main.html', **templateData)

# The function below is executed when someone requests a URL with the pin number and action in it:
@app.route("/<changePin>/<action>")
def action(changePin, action):
   # Convert the pin from the URL into an integer:
   changePin = int(changePin)
   # Get the device name for the pin being changed:
   deviceName = pins[changePin]['name']
   # If the action part of the URL is "on," execute the code indented below:
   if action  == "stop":
      # Set the pin high:
      pi.set_servo_pulsewidth(changePin,0)
      # Save the status message to be passed into the template:
      message = "Turned " + deviceName + " on."
  
   if action  == "zero":
      pi.set_servo_pulsewidth(changePin,1000)      
      message = "Turned " + deviceName + " off."

   if action  == "mid":
      pi.set_servo_pulsewidth(changePin,1500)
      message = "Turned " + deviceName + " off."

   if action  == "last":
      pi.set_servo_pulsewidth(changePin,2000)
      message = "Turned " + deviceName + " off."


   # For each pin, read the pin state and store it in the pins dictionary:
   for pin in pins:
      pins[pin]['state'] = pi.read(pin)

   # Along with the pin dictionary, put the message into the template data dictionary:
   templateData = {
      'pins' : pins
   }

   return render_template('main.html', **templateData)

if __name__ == "__main__":
   app.run(host='0.0.0.0', port=80, debug=True)
