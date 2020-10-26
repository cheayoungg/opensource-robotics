#serial_comm.py
#!/usr/bin/ python2
import serial
import time

ser = serial.Serial('/dev/ttyACM0', 9600, timeout=5)
# read from Arduino
input = ser.read()
print ("Read input " + input.decode("utf-8") + " from Arduino")
ser.write(b'A')

while 1:
       # write something back
       # ser.write(b'A')
        input = ser.read()
        if (input != None):
          ser.write(b'A')
          print( 'input:{}, ascii:{} '.format(ord(input), input))

