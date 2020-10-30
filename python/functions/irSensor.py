# irSensor.py
# irSensor function

# Pin 
rightIrSensorPin = 0
leftIrSensorPin =1


# Set analog pins 0 and 1 to be analog input pins
def readyIrSensor(board):
    board.set_pin_mode(rightIrSensorPin,board.INPUT,board.ANALOG)
    board.set_pin_mode(leftIrSensorPin,board.INPUT,board.ANALOG)

def readIrSensor(board,pin):
    value = 1023 - board.analog_read(pin)
    return value

def colorFinder(sensorValue):
    THRESHOLD_IR = 800
    WHITE = 1
    BLACK = 0
    color = WHITE if sensorValue > THRESHOLD_IR else BLACK
    return color

def getSensorValues(board):
    # Read IR sensor values
    rightIrSensor = readIrSensor(board,rightIrSensorPin)
    leftIrSensor = readIrSensor(board,leftIrSensorPin)
    return {'right' : rightIrSensor,'left' : leftIrSensor}

def getColors(board):
    # Find color
    rightIrSensor = readIrSensor(board,rightIrSensorPin)
    leftIrSensor = readIrSensor(board,leftIrSensorPin)
    colorRightSensor = colorFinder(rightIrSensor)
    colorLeftSensor  = colorFinder(leftIrSensor)
    return {'right' : colorRightSensor, 'left': colorLeftSensor}

 
   
  