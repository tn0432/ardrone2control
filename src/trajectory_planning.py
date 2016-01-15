#!/usr/bin/env python
import roslib; 
roslib.load_manifest('ardrone2control')
import rospy
import math
import time
import numpy as np
import std_msgs.msg
import sys

import tf
from tf.transformations import euler_from_quaternion

from sensor_msgs.msg import Image, RegionOfInterest, CameraInfo
from geometry_msgs.msg import Twist
from geometry_msgs.msg import Wrench
from sensor_msgs.msg import Range


from visualization_msgs.msg import Marker
from visualization_msgs.msg import MarkerArray
from geometry_msgs.msg import Point
from geometry_msgs.msg import Vector3
from std_msgs.msg import ColorRGBA
from geometry_msgs.msg import Quaternion

import random
    #---------- Disturbance Setup ----------#
disturbX = []
disturbY = []
disturbZ = []
    
with open('/home/trungnguyen/fuerte_workspace/TrackingController/ExternalDisturbance.txt') as f:
        for line in f:
            data = line.split()
            disturbX.append(float(data[0]))
            disturbY.append(float(data[1]))
            disturbZ.append(float(data[2]))

#----- Training by setting the time point of trajectory -----#
def CirclePath(OriginX, OriginY, OriginZ, Radius, time):
    x = round(OriginX + Radius * math.cos(2.0 * time / 100.0 * 2 * math.pi- math.pi/2),5);
    y = round(OriginY + Radius * math.sin(2.0 * time / 100.0 * 2 * math.pi- math.pi/2),5);
    #print (x,y)
    return (x, y, OriginZ)
 
#========== Main function ==========#
if __name__ == '__main__':
    TimeStart = 0
    rospy.init_node("TestGeometricGCTrackingController")
    listener = tf.TransformListener()
  
    #---------- Create a publisher so that we can output command velocities ----------#
    cmd_vel_publisher = rospy.Publisher('/cmd_vel', Twist)
    cmd_forcetorque_publisher = rospy.Publisher('/cmd_forcetorque', Wrench)    

    #---------- Create a publisher so that we can draw the vector of the controller in rviz ----------#
    guiding_vectors_publisher = rospy.Publisher('/guidingVectors', MarkerArray)
  
    GoalTf = tf.TransformBroadcaster()

    ith = 0

    last_time = 0
    current_time = rospy.get_time()  
    rate = rospy.Rate(50.0)
    while not rospy.is_shutdown():

        current_time = rospy.get_time()
        dt = (current_time - last_time)
        last_time = current_time
        #----- Set Reference -----#
        XOrigin = 0
        YOrigin = 0
        ZOrigin = 0.7
        Radius = 1
    
        if TimeStart == 0:
           TimeStart = current_time    
        Time = current_time - TimeStart
        
        #----- Create a goal frame for trajectory tracking -----#
        (Xref, Yref, Zref) = CirclePath(XOrigin, YOrigin, ZOrigin, Radius, Time)
	#(Xref, Yref, Zref) = (0,0,1)
        if (Yref - YOrigin)>=0: 
           YawGoalRotate = math.pi/2 + math.acos((Xref-XOrigin)/Radius)
        else:
           YawGoalRotate = math.pi/2 - math.acos((Xref-XOrigin)/Radius)
	#YawGoalRotate = 0
        
        GoalTf.sendTransform((Xref, Yref, Zref),tf.transformations.quaternion_from_euler(0.0,0.0, YawGoalRotate),rospy.Time.now(),"/goal","/world") 
    
 
       
        rate.sleep()



