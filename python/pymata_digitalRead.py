# pymata_digitalRead.py
# PyMata digital read example
from PyMata.pymata import PyMata
import time

# Pin no.
pinNo = 2
# connection port
PORT = '/dev/ttyACM0'
# Create PyMata instance
board = PyMata(PORT, verbose=True)

# Set digital pin 13 to be  an output port
board.set_pin_mode(pinNo,board.INPUT,board.DIGITAL)

while True:
  pin = board.digital_read(pinNo)
  print 'pin state: {}'.format(pin)
  time.sleep(0.5)

