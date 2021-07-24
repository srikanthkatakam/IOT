import time
from flask import Flask, render_template, request
app = Flask(__name__)
import pigpio
pi = pigpio.pi()


# Set each pin as an output and make it low:
pi.set_mode(2, pigpio.OUTPUT)
pi.set_mode(13, pigpio.OUTPUT)
pi.set_mode(19, pigpio.OUTPUT)
pi.set_mode(26, pigpio.OUTPUT)
pi.set_mode(16, pigpio.OUTPUT)
pi.set_mode(20, pigpio.OUTPUT)
pi.set_mode(21, pigpio.OUTPUT)
pi.write(13,1)
pi.write(16,1)

# Create a dictionary called pins to store the pin number, name, and pin state:
pins = {
   2 : {'name' : 'GPIO 2', 'state' : pigpio.PUD_DOWN}
   }

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

@app.route("/<action>")

def action(action):
   # If the action part of the URL is "STOP," execute the code indented below:

   if action  == "sstop":
      pi.set_servo_pulsewidth(2,0)

  
   if action  == "zero":
      pi.set_servo_pulsewidth(2,1000)      


   if action  == "mid":
      pi.set_servo_pulsewidth(2,1500)


   if action  == "last":
      pi.set_servo_pulsewidth(2,2000)

   if action  == "forward":
      pi.write(19,1)
      pi.write(20,1)
      pi.write(26,0)
      pi.write(21,0)

   if action  == "reverse":
      pi.write(26,1)
      pi.write(21,1)
      pi.write(19,0)
      pi.write(20,0)

   if action  == "right":
      pi.write(19,1)
      pi.write(20,0)
      pi.write(26,0)
      pi.write(21,0)
   
   if action  == "left":
      pi.write(19,0)
      pi.write(20,1)
      pi.write(26,0)
      pi.write(21,0)

   if action  == "rstop":
      pi.write(19,0)
      pi.write(20,0)
      pi.write(26,0)
      pi.write(21,0)

   # For each pin, read the pin state and store it in the pins dictionary:
   # for pin in pins:
      # pins[pin]['state'] = pi.read(pin)

   # Along with the pin dictionary, put the message into the template data dictionary:
   templateData = {
      'pins' : pins
   }

   return render_template('main.html', **templateData)

if __name__ == "__main__":
   app.run(host='0.0.0.0', port=80, debug=True)
