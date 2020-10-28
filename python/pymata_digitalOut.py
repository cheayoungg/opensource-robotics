# pymata_digitalOut.py
# PyMata digital out example
from PyMata.pymata import PyMata
import time

# Pin no.
pinNo = 8
# connection port
PORT = '/dev/ttyACM0'
# Create PyMata instance
board = PyMata(PORT, verbose=True)

# Set digital pin 8 to be  an output port
board.set_pin_mode(pinNo,board.OUTPUT,board.DIGITAL)

# Blink for 10 times
for x in range(10):
  board.digital_write(pinNo,1)
  time.sleep(.5)
  board.digital_write(pinNo,0)
  time.sleep(.5)

# Close PyMata
board.close()
