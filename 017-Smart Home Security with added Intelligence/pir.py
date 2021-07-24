import time
import urllib2
import pigpio
pi = pigpio.pi()

PIR_PIN = 18  # PIR Motion Sensor pin
EVENT = 'dooropen'
BASE_URL = 'https://maker.ifttt.com/trigger/'
KEY = 'OEZTLJDK7yXgD8p9JND97'

# Configure the PIGPIO pin
pi.set_mode(PIR_PIN, pigpio.INPUT)
pi.set_pull_up_down(PIR_PIN, pigpio.PUD_DOWN)

def send_event():
    response = urllib2.urlopen(BASE_URL + EVENT + '/with/key/' + KEY)
    print(response.read())

try:
    while True:
        if pi.read(PIR_PIN) == 0:
            # Its light (door open)
            send_event()
            # Do nothing until the doior is closed again
            while pi.read(PIR_PIN) == 0:
                time.sleep(0.1)
            # Do nothing for a further minute anyway
            print("Wait a minute")
            time.sleep(1)
            print("Monitoring again")

finally:
    print('Cleaning up PIGPIO')
    pigpio.cleanup()
