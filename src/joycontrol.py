#!/usr/bin/env python
import rospy
from std_msgs.msg import String
from std_msgs.msg import Int8
from sensor_msgs.msg import Joy
from geometry_msgs.msg import Twist

rospy.set_param('mode','0')
cmd_vel_publisher = rospy.Publisher('/cmd_vel', Twist)
ModeCommand = Int8()
ModeCommand = 0

def joy_callback(data):
    global TwistCommand, YawDesired, ModeCommand
    
    TwistCommand = Twist()
    TwistCommand.linear.x = data.axes[1]*0.25
    TwistCommand.linear.y = data.axes[0]*0.25



    # Up - down
    stepZ = 0.01
    stepZHigh = 0.1
    stepYaw = 0.01
    if data.buttons[2] == 1.0:
	TwistCommand.linear.z = stepZ
    if data.buttons[1] == 1.0: # button 2
        TwistCommand.linear.z = -stepZ

    if data.buttons[10] == 1.0: # button11 
	TwistCommand.linear.z = stepZHigh
    if data.buttons[9] == 1.0: # button10
        TwistCommand.linear.z = -stepZHigh
    print TwistCommand.linear.z
    # Yaw left-right
    if data.buttons[3] == 1.0:
	YawDesired = stepYaw

    if data.buttons[4] == 1.0:
	YawDesired = -stepYaw

    # Mode
    if data.buttons[6] == 1.0:
	ModeCommand = ModeCommand + 1
	if ModeCommand > 2:
	   ModeCommand = 0
	rospy.set_param('/mode',str(ModeCommand))

    TwistCommand.angular.z = YawDesired

    print "Mode", ModeCommand
    print TwistCommand
    cmd_vel_publisher.publish(TwistCommand)  
    TwistCommand.angular.z = 0
    YawDesired = 0
    #mode_publisher.publish(ModeCommand)

if __name__ == '__main__':
    global TwistCommand, YawDesired
    YawDesired = 0;
   
    TwistCommand = Twist()
    rospy.init_node('joy_listener', anonymous=True)
 
    # Create a ROS subcriber
    rospy.Subscriber("joy", Joy, joy_callback)
    rospy.set_param('/mode','0')
    #
    # Create a ROS publisher
    rospy.spin()    

