
# serial_comm_binary_3Bytes.py
# sketch_serial_comm_3Bytes.ino
# 3 BYTES (unsigned character) communication between Raspberry pi and Arduino

#!/usr/bin/ python2

import serial
import time
import struct

ser = serial.Serial('/dev/ttyACM0', 9600, timeout=1)
print('wait until serial port is ready...')
   ## wait until serial is ready
time.sleep(2)

# vairalbes
dataReceivedCount = 0 # data received count
NumOfData = 3         # Number of data to send to Arduino
dataSendReadyFlag = 1 # set dataSend flag
data = 0 # data to send
# send 3 bytes of data
def sendDataNumber3(serial,data,format_string):
        packedData2Send = struct.pack(format_string,*data)
        serial.write(packedData2Send)


while 1:
        if dataSendReadyFlag:
                data += 1
                rawData = (data,data+1,data+2) 
                sendDataNumber3(ser,rawData,'BBB')
                dataSendReadyFlag=0 # reset dataSendReady flag

        if(ser.inWaiting() ==  NumOfData):
                dataSendReadyFlag=1 # set dataSendReady flag
                dataReceivedCount +=1
                readBytes = ser.read(NumOfData)
                decode = struct.unpack_from('BBB',readBytes)
                print 'count:%s, decode: %s' % (dataReceivedCount,decode)
                time.sleep(0.5)

