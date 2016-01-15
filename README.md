# ardrone2control
Trajectory Control of AR.Drone ver 2.0 quadrotor model

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
The C code project is in folder ardrone2islab. Steps to complie and upload the code to AR.Drone are presented following: 
1. Prepare your code for attitude control to upload to AR.Drone<br/>
2. [First time install compiler] do it <br/>
3. Open new terminal to setup compiler, <code>codesourcery-arm-2009q3.sh</code><br/>
4. cd to your folder ardrone2islab, <code>make</code> , it will generate an executive file fly_ardrone_pid<br/>
5. Connect to AR.Drone wifi, the IP of AR.Drone: 192.168.1.1<br/>
6. <code>ftp 192.168.1.1</code> <br/>
7. Until see connection successful, Press Enter<br/>
8. <code>put fly_ardrone_pid</code><br/> Now your file is in AR.Drone

How to run your file on AR.Drone board?
1. <code> telnet 192.168.1.1 </code> <br/>
2. 


### ROS node for ground station

### How to conduct the real-time experiment?

### Status
[January 2016] Upload the code project to github
