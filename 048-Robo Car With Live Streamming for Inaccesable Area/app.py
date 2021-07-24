# -*- coding: utf-8 -*-
import RPi.GPIO as GPIO
import time
from flask import Flask, render_template, request
app = Flask(__name__)

GPIO.setmode(GPIO.BCM)

# Set each pin as an output and make it low:
GPIO.setup(2, GPIO.OUT)
GPIO.setup(13, GPIO.OUT)
GPIO.setup(19, GPIO.OUT)
GPIO.setup(26, GPIO.OUT)
GPIO.setup(16, GPIO.OUT)
GPIO.setup(20, GPIO.OUT)
GPIO.setup(21, GPIO.OUT)
GPIO.output(13, GPIO.HIGH)
GPIO.output(16, GPIO.HIGH)

p = GPIO.PWM(2, 50)
p.start(2.5)

@app.route("/")
def main():
   
   # Pass the template data into the template main.html and return it to the user
   return render_template('main.html')

# The function below is executed when someone requests a URL with action in it:
@app.route("/<action>")

def action(action):
   try:
      if action  == "sstop":
         p.ChangeDutyCycle(0)  

      if action  == "zero":
         p.ChangeDutyCycle(2.5) #0°     

      if action  == "mid":
         p.ChangeDutyCycle(7.5) #Neutral (90°)

      if action  == "last":
         p.ChangeDutyCycle(12.5) #180°
   except keyboardinterrupt:
      p.stop()
      
   if action  == "forward":
      GPIO.output(19,1)
      GPIO.output(20,1)
      GPIO.output(26,0)
      GPIO.output(21,0)

   if action  == "reverse":
      GPIO.output(26,1)
      GPIO.output(21,1)
      GPIO.output(19,0)
      GPIO.output(20,0)

   if action  == "right":
      GPIO.output(19,1)
      GPIO.output(20,0)
      GPIO.output(26,0)
      GPIO.output(21,0)
   
   if action  == "left":
      GPIO.output(19,0)
      GPIO.output(20,1)
      GPIO.output(26,0)
      GPIO.output(21,0)

   if action  == "rstop":
      GPIO.output(19,0)
      GPIO.output(20,0)
      GPIO.output(26,0)
      GPIO.output(21,0)

   # For each pin, read the pin state and store it in the pins dictionary

   return render_template('main.html')

if __name__ == "__main__":
   app.run(host='0.0.0.0', port=8085, debug=True)

