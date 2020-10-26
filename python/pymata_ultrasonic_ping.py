# pymata_ultrasonic_ping.py
# ultrasonic sensor test
# FirmataPlus is required on Arduino.

import time
from PyMata.pymata import PyMata

PORT = '/dev/ttyACM0'
board = PyMata(PORT,verbose=True)
pinNo = 12
trig=echo=pinNo
board.sonar_config(trig,echo)
time.sleep(1)


while True:
  data = board.get_sonar_data()
  dist = data[pinNo]
  print 'distance: {} cm'.format(dist[1])
  time.sleep(.1)


