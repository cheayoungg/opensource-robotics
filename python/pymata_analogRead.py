# pymata_analogRead.py
# PyMata analog read example
from PyMata.pymata import PyMata
import time

# Pin no. A0
pinNo = 0
# connection port
PORT = '/dev/ttyACM0'
# Create PyMata instance
board = PyMata(PORT, verbose=True)

# Set digital pin 13 to be  an output port
board.set_pin_mode(pinNo,board.INPUT,board.ANALOG)

while True:
  pin = board.analog_read(pinNo)
  print 'analog read : A0: {}'.format(pin)
  time.sleep(0.5)
