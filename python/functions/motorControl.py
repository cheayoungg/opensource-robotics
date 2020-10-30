# motorControl.py
# Motor Control Function

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

# Set motor pwm & directions
def setMotor(board,PWM, DIR):
    board.set_pin_mode(PWM,board.PWM,board.DIGITAL)
    board.set_pin_mode(DIR,board.OUTPUT,board.DIGITAL)
    #time.sleep(0.5)

def setRightAndLeftMotors(board):
    setMotor(board,PWMA,DIRA)
    setMotor(board,PWMB,DIRB)

# Drive Motor
def driveMotor(board,motor, direction, speed):
    if (motor == MOTOR_RIGHT):
        board.analog_write(PWMA,speed)
        board.digital_write(DIRA,direction)
    elif (motor == MOTOR_LEFT):
        board.analog_write(PWMB,speed)
        board.digital_write(DIRB,direction)

# Robot Motion Control
def robotForward(board,power):  
    driveMotor(board,MOTOR_RIGHT,CW,power)
    driveMotor(board,MOTOR_LEFT,CCW,power)

def robotBackward(board,power):  
    driveMotor(board,MOTOR_RIGHT,CCW,power)
    driveMotor(board,MOTOR_LEFT,CW,power)

def robotRight(board,power):    
    driveMotor(board,MOTOR_RIGHT,CCW,power)
    driveMotor(board,MOTOR_LEFT,CCW,power)

def robotLeft(board,power):    
    driveMotor(board,MOTOR_RIGHT,CW,power)
    driveMotor(board,MOTOR_LEFT,CW,power)

def robotStop(board):
    driveMotor(board,MOTOR_RIGHT,CW,0)
    driveMotor(board,MOTOR_LEFT, CW,0)

    