#pymata_motorControl.py

# PyMata analog read for IR sensor example
from PyMata.pymata import PyMata
import time

# Pin 
PWMA = 3      # PWM control (speed) for motor A - RIGHT WHEEL
DIRA = 12     # Direction control for motor A
PWMB = 11     # PWM control (speed) for motor B - LEFT WHEEL
DIRB = 13     # Direction control for motor B

# Motor settings
CW = 1   # clockwise
CCW = 0  # counter-clockwise
MOTOR_LEFT = 0 # left motor
MOTOR_RIGHT = 1 # right motor

# connection port
PORT = '/dev/ttyACM0'
# Create PyMata instance
board = PyMata(PORT, verbose=True)

# Set motor pwm & directions
def setMotor(PWM, DIR):
    board.set_pin_mode(PWM,board.PWM,board.DIGITAL)
    board.set_pin_mode(DIR,board.OUTPUT,board.DIGITAL)
    #time.sleep(0.5)

def setRightAndLeftMotors():
    setMotor(PWMA,DIRA)
    setMotor(PWMB,DIRB)


# Drive Motor
def driveMotor(motor, direction, speed):
    if (motor == MOTOR_RIGHT):
        board.analog_write(PWMA,speed)
        board.digital_write(DIRA,direction)
    elif (motor == MOTOR_LEFT):
        board.analog_write(PWMB,speed)
        board.digital_write(DIRB,direction)

# Robot Motion Control
def robotForward(power):  
    driveMotor(MOTOR_RIGHT,CW,power)
    driveMotor(MOTOR_LEFT,CCW,power)

def robotBackward(power):  
    driveMotor(MOTOR_RIGHT,CCW,power)
    driveMotor(MOTOR_LEFT,CW,power)

def robotRight(power):    
    driveMotor(MOTOR_RIGHT,CCW,power)
    driveMotor(MOTOR_LEFT,CCW,power)

def robotLeft(power):    
    driveMotor(MOTOR_RIGHT,CW,power)
    driveMotor(MOTOR_LEFT,CW,power)

def robotStop():
    power = 0
    driveMotor(MOTOR_RIGHT,CW,power)
    driveMotor(MOTOR_LEFT,CW,power)

############### PROGRAM  ############################
# Set Right and Left Motors
setRightAndLeftMotors()

while True:

    robotForward(0)
    