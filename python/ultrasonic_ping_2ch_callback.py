# ultrasonic_ping_2ch_callback.py

# callback function is tested

import time
from PyMata.pymata import PyMata

PORT = '/dev/ttyACM0'
board = PyMata(PORT,verbose=True)
trig=8
echo=9

def callback(res):
    print 'callback:{}'.format(res)



board.sonar_config(trigger_pin=trig,echo_pin=echo,cb=callback,ping_interval=33)
time.sleep(1)


while True:
  data = board.get_sonar_data()
  dist = data[trig]
  print 'distance: {} cm'.format(dist[1])
  time.sleep(.5)
