# pymata_IRsensor.py


# PyMata analog read for IR sensor example
from PyMata.pymata import PyMata
import time

# Pin 
rightIrSensorPin = 0
leftIrSensorPin =1
# connection port
PORT = '/dev/ttyACM0'
# Create PyMata instance
board = PyMata(PORT, verbose=True)
time.sleep(2)

# Set analog pins 0 and 1 to be analog input pins
board.set_pin_mode(rightIrSensorPin,board.INPUT,board.ANALOG)
board.set_pin_mode(leftIrSensorPin,board.INPUT,board.ANALOG)

def readIrSensor(pin):
    value = 1023 - board.analog_read(pin)
    return value

def colorFinder(sensorValue):
    THRESHOLD_IR = 800
    WHITE = 1
    BLACK = 0
    color = WHITE if sensorValue > THRESHOLD_IR else BLACK
    return color


while True:
  
  # Read IR sensor values
  rightIrSensor = readIrSensor(rightIrSensorPin)
  leftIrSensor = readIrSensor(leftIrSensorPin)
  
  # Find color
  colorRightSensor = colorFinder(rightIrSensor)
  colorLeftSensor  = colorFinder(leftIrSensor)

  print 'rightIR:{}, leftIR:{}'.format(rightIrSensor, leftIrSensor)
  print 'rightSensorColor:{}, leftSensorColor:{}'.format(colorRightSensor,colorLeftSensor) 
  


