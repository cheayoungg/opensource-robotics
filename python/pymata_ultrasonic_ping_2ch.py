# pymata_ultrasonc_ping_2ch.py
# 2 ch input : trig, echo
# FirmataPlus is required on Arduino

import time
from PyMata.pymata import PyMata

PORT = '/dev/ttyACM0'
board = PyMata(PORT,verbose=True)
trig=9
echo=8
board.sonar_config(trig,echo)
time.sleep(1)


while True:
  data = board.get_sonar_data()
  dist = data[trig]
  print 'distance: {} cm'.format(dist[1])
  time.sleep(.1)
