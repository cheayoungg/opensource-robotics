# -*- coding: utf-8 -*-
"""
Created on 

@author: Byoung L.
"""
#Import Libraries:
import vrep                #V-rep library
import sys
import time                #used to keep track of time
import numpy as np         #array library

#Pre-Allocation
vrep.simxFinish(-1) # just in case, close all opened connections

#Connect to V-rep server
clientID=vrep.simxStart('127.0.0.1',19999,True,True,5000,5)

if clientID!=-1:  #check if client connection successful
    print ('Connected to remote API server')
else:
    print ('Connection not successful')
    sys.exit('Could not connect')

#Start simulation with a synchronous mode
vrep.simxSynchronous(clientID,True) # Enable the synchronous mode (Blocking function call)
# Simulation starts and the first simulation step waits for a trigger before being executed
vrep.simxStartSimulation(clientID, vrep.simx_opmode_oneshot)

#Retrieve motor handles
errorCodeL,left_motor_handle=vrep.simxGetObjectHandle(clientID, \
                                'leftMotor',vrep.simx_opmode_oneshot_wait)
errorCodeR,right_motor_handle=vrep.simxGetObjectHandle(clientID, \
                                 'rightMotor',vrep.simx_opmode_oneshot_wait)
# Retrieve sensor handle
errorCodeS,sensor_handle=vrep.simxGetObjectHandle(clientID, \
                                 'sensingNose',vrep.simx_opmode_oneshot_wait)
# Request Proximity sensor values in Streaming mode
errorCodeR,detectionState,detectedPoint,detectedObjectHandle,detectedSurfaceNormalVector= \
                    vrep.simxReadProximitySensor(clientID,sensor_handle,vrep.simx_opmode_streaming)

#Set initial simulation properties
#wheelVelocityLeft = wheelVelocityRight    # initial wheel veloicties
wheelVelocityLeft = wheelVeolocityRight = 1
simulationStartTime = time.time()           # simulation start time
simulationTime = 30                         # simulation time

# motion functions
def moveBackward(speed):
    global wheelVelocityRight, wheelVelocityLeft
    wheelVelocityRight =   -speed/2
    wheelVelocityLeft =    -speed/8
    #time.sleep(3)

def moveForward(speed):
    global wheelVelocityRight, wheelVelocityLeft
    wheelVelocityRight =   speed
    wheelVelocityLeft =    speed

def moveRight(speed):
    global wheelVelocityRight, wheelVelocityLeft
    wheelVelocityRight =   -0.6*speed
    wheelVelocityLeft =    speed

# simulation loop
while (time.time() - simulationStartTime) < simulationTime:
    #Loop Execution
    # Trigger simulation and simulation begins in a synchronous manner
    vrep.simxSynchronousTrigger(clientID);
    # After this call(simxGetPingTime), the first simulation step is finished (Blocking function call)
    # Now we can safely read all streamed values
    vrep.simxGetPingTime(clientID)

    # Read sensor measurements from buffer
    errorCode,detectionState,detectedPoint,detectedObjectHandle,detectedSurfaceNormalVector= \
    vrep.simxReadProximitySensor(clientID,sensor_handle,vrep.simx_opmode_buffer)
    # average measurements
    #sensor_val = np.linalg.norm(detectedPoint)
   
    sensor_val = detectionState # detect state : True or False
    print 'errCode:{},detectionState:{},detectedPoint:{}'.format(errorCode,detectionState,detectedPoint)

    # Calculate control action
    # if obstacle is detected, then turn.
    if sensor_val == True:
       moveRight(10)
        
    else:
        #wheelVelocityLeft = wheelVelocityRight = 1
        moveForward(2)

    # Set Joint Veolocities
    # communication to V-rep is temporalally on a pause to send multiple joint
    # velocities simultaneously
    vrep.simxPauseCommunication(clientID,1); #pause communication
    # Set left-joint velocity
    errorCode=vrep.simxSetJointTargetVelocity(clientID,left_motor_handle, \
                                         wheelVelocityLeft, vrep.simx_opmode_oneshot)
    # Set right-joint velocity
    errorCode=vrep.simxSetJointTargetVelocity(clientID,right_motor_handle, \
                                         wheelVelocityRight, vrep.simx_opmode_oneshot)
    vrep.simxPauseCommunication(clientID,0); #resume communication

    # Display simulation result
    elapsedTime = time.time()-simulationStartTime #elaped time since start of simulation
    #print 'simTime:{0:2.1f}, sensorValue:{1:2.1f}'.format(elapsedTime, sensor_val)
    print ('simTime:{}, sensorValue:{}'.format(elapsedTime, sensor_val))
    print ('wheelVelocityLeft:{0:2.1f}, wheelVelocityRight:{1:2.1f}' \
                              .format(wheelVelocityLeft, wheelVelocityRight))

#############################
#Post ALlocation- stop robot#
#############################
# Set left-joint velocity to zero
errorCode=vrep.simxSetJointTargetVelocity(clientID,left_motor_handle,0, vrep.simx_opmode_oneshot)
# Set right-joint velocity to zero
errorCode=vrep.simxSetJointTargetVelocity(clientID,right_motor_handle,0, vrep.simx_opmode_oneshot)
# Stop simulation
vrep.simxStopSimulation(clientID, vrep.simx_opmode_oneshot)
# Before closing the connection to V-REP, make sure that the last command sent out had time to arrive. You can guarantee this with (for example):
vrep.simxGetPingTime(clientID)
# Now close the connection to V-REP:
vrep.simxFinish(clientID)
print ('{0}'.format("control complete and connection lost ..."))





