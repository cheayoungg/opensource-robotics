# ultrasonicSensor.py

def configure(board, trig, echo):
    trig_pin = trig
    echo_pin = echo
    board.sonar_config(trigger_pin=trig_pin,echo_pin=echo_pin,ping_interval=33)

def getDistance(board,trig):
    sonar_data = board.get_sonar_data()
    distance = sonar_data[trig]
    return distance[1]
    
