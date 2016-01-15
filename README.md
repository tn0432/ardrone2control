# ardrone2control
Project: Trajectory Control of AR.Drone ver 2.0 quadrotor model </br>
Authors: Trung Nguyen and Oscar De Silva </br>
Intelligent Systems Lab - Memorial University of Newfoundland - Canada 

The project can be used for researching quadrotor with specific topics:
- Trajectory control or path following
- State estimation (Attitude and Heading Reference System)
- Trajectory generation or path planning

### Equipments
1. AR.Drone quadrotor ver 2.0 
2. Ground station (desktop or laptop) with ROS Fuerte installed
3. Optitrack motion capture system (or camera mounting on the ceiling) 
4. Joystick (optional)

### Driver for AR.Drone ver 2.0
The C code project is in folder ardrone2islab: <link>https://github.com/tn0432/ardrone2islab.git</link> <br/>
Steps to complie and upload the code to AR.Drone are presented following: 
1. Prepare your code for attitude control to upload to AR.Drone<br/>
2. [First time install compiler] Please follow the steps in [the following link](http://taghof.github.io/Navigation-for-Robots-with-WIFI-and-CV/blog/2012/01/13/Compiling-Code-For-The-ARDrone/) to setup the compiler as well as test simple program uploading to AR.Drone <br/> 
3. Open new terminal to setup compiler, <code>codesourcery-arm-2009q3.sh</code><br/>
4. cd to your folder ardrone2islab, <code>make</code> , it will generate an executive file fly_ardrone_pid<br/>
5. Connect to AR.Drone wifi, the IP of AR.Drone: 192.168.1.1<br/>
6. <code>ftp 192.168.1.1</code> <br/>
7. Until see connection successful, Press Enter<br/>
8. <code>put fly_ardrone_pid</code>. Now your file is in AR.Drone

<b>How to run your file on AR.Drone board?</b><br/>
1. Open new terminal or Ctrl+Alt+T<br/>
2. <code> telnet 192.168.1.1 </code> <br/>
3. <code> cd ./data/video/fly_ardrone_pid </code> <br/>

<b>How to restart AR.Drone back to original manufacturing? or How to make emergency stop for AR.Drone during flight?</b><br/>
Simple method is unpluging battery from AR.Drone and pluging it in again, or use following commands: <br/>
1. Open new terminal or Ctrl+Alt+T<br/>
2. <code> telnet 192.168.1.1 </code> <br/>
3. <code> cd ./sbin/reboot </code> <br/>

### ROS node for ground station
The optitrack motion capture system will estimate the current position of AR.Drone and send that position to the ground station. The ROS driver for motion capture system is [mocap_optitrack](http://wiki.ros.org/mocap_optitrack). In a ground station (your desktop), a ROS node (communication_node_pid_semitrajectorycontrol3) is used to calculate control signals sent to AR.Drone. The control signals are desired Thrust, Pitch, Roll, Yaw.

In the beginning, AR.Drone is at manual control state with very small value of Thrust. I use joystick for manual control. When it transfers to trajectory control state, the value of Thrust is set by the error of height. AR.Drone will start to track the desired circle trajectory. So, <b>please focus on the safety issues when the flight mode is transfered to trajectory control!</b>  

### Status
[January 2016] Upload the code project to github

### Reference
http://taghof.github.io/Navigation-for-Robots-with-WIFI-and-CV/blog/2012/01/13/Compiling-Code-For-The-ARDrone/ <br/>
http://wiki.paparazziuav.org/wiki/AR_Drone_2 <br/>
http://blog.perquin.com/blog/category/ardrone/ <br/>
