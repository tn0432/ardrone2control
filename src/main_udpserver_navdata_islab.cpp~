#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "udp.h"
#include <pthread.h>
// Include the ROS C++ APIs
#include <ros/ros.h>
#include <ardrone_islab/Navdataislab.h>

void diep(const char *s)
{
  perror(s);
  exit(1);
}
//structure for ahrs
struct att_struct {
  double tm; //timestamp in sec
  double tm_pre;
  float   frame;

  //pitch estimates in radians, positive is pitch down (fly forward)
  float pitch_w;  //=sum(gx * dt)
  float pitch_a;  //=pitch(az,ax)
  float pitch;    //kalman pitch estimate from gy and pitch_a

  //roll estimates in radians, positive is roll right (fly rightward)
  float roll_w;   //=sum(gy * dt)
  float roll_a;   //=roll(az,ay)
  float roll;     //kalman roll estimate from gx and roll_a
  
  //yaw estimate, positive is yaw left
  float yaw_w;      //=sum(gz * dt)
  float yaw_m;      //=sum(gz * dt)
  float yaw;      //=sum(gz * dt)
  
  double dt;  // time since last navdata sample in sec
  double dt2; // time consumed by the ahrs calculations

  double q_est[4];
  double b_est[3];

  //copy of physical navdata values
  	double ts;  // navdata timestamp in sec
  	float hraw;    // height above ground in [cm] 
  	char h_meas;// 1=height was measured in this sample, 0=height is copy of prev value
  	float ax;   // acceleration x-axis in [G] front facing up is positive         
	float ay;   // acceleration y-axis in [G] left facing up is positive                
	float az;   // acceleration z-axis in [G] top facing up is positive  
           
	float wx;   // gyro value x-axis in [rad/sec] right turn, i.e. roll right is positive           
	float wy;   // gyro value y-axis in [rad/sec] right turn, i.e. pirch down is positive                     
	float wz;   // gyro value z-axis in [rad/sec] right turn, i.e. yaw left is positive

	float magX;
	float magY; 
	float magZ;

	float pressure;

	float rotX;
	float rotY;
	float rotZ;

	float altd;

	float motor1;
	float motor2;
	float motor3;
	float motor4;
};

int main(int argc, char **argv)
{
  udp_struct udp;
  ardrone_islab::Navdataislab navdata;

  int msglen;
  char buf[512];
  char * pch;
  ardrone_islab::Navdataislab navdataformat;
  ros::init(argc, argv, "ardrone_ground_node_server");
  ros::NodeHandle n;
  ros::Rate loop_rate(300);
  
  ros::Publisher navdata_pub = n.advertise<ardrone_islab::Navdataislab>("ardrone/navdata",1000);
  
  printf("check1");
  // Broadcast a simple log message
  ROS_INFO_STREAM("Start ROS node!");
  
  if(udpServer_Init(&udp,9930,0)) diep("udpServer_Init");
  printf("check2");
  while (ros::ok())
  {

	int cnt=0;
   	do {
      		msglen = udpServer_Receive(&udp, buf, 512);
      		cnt++;
    	} while(msglen<=0);
 
        pch = strtok (buf," ,");
        int countpart = 0;
	
	//printf("%d",len(buf));
  	while (countpart<=42)
  	{
 		if (countpart == 0) navdata.tm = atof(pch);
		if (countpart == 1) navdata.tm_pre = atof(pch);
		if (countpart == 2) navdata.frame = atof(pch);
  		if (countpart == 3) navdata.pitch_w = atof(pch);
  		if (countpart == 4) navdata.pitch_a = atof(pch);
  		if (countpart == 5) navdata.pitch = atof(pch);
  		if (countpart == 6) navdata.roll_w = atof(pch);
  		if (countpart == 7) navdata.roll_a = atof(pch);
  		if (countpart == 8) navdata.roll = atof(pch);
  		if (countpart == 9) navdata.yaw_w = atof(pch);
  		if (countpart == 10) navdata.yaw_m = atof(pch);
  		if (countpart == 11) navdata.yaw = atof(pch);
  		if (countpart == 12) navdata.dt = atof(pch);
		if (countpart == 13) navdata.dt2 = atof(pch);
  		if (countpart == 14) navdata.q_est[0] = atof(pch);
  		if (countpart == 15) navdata.q_est[1] = atof(pch);
  		if (countpart == 16) navdata.q_est[2] = atof(pch);
  		if (countpart == 17) navdata.q_est[3] = atof(pch);
  		if (countpart == 18) navdata.b_est[0] = atof(pch);
  		if (countpart == 19) navdata.b_est[1] = atof(pch);
  		if (countpart == 20) navdata.b_est[2] = atof(pch);

  		if (countpart == 21) navdata.ts = atof(pch);
  		if (countpart == 22) navdata.hraw = atof(pch);
  		if (countpart == 23) navdata.h_meas = atof(pch);
  		if (countpart == 24) navdata.ax = atof(pch);         
  		if (countpart == 25) navdata.ay = atof(pch);               
  		if (countpart == 26) navdata.az = atof(pch);  
  		if (countpart == 27) navdata.wx = atof(pch);        
  		if (countpart == 28) navdata.wy = atof(pch);                  
  		if (countpart == 29) navdata.wz = atof(pch);
  		if (countpart == 30) navdata.magX = atof(pch);
  		if (countpart == 31) navdata.magY = atof(pch);
  		if (countpart == 32) navdata.magZ = atof(pch);
  		if (countpart == 33) navdata.pressure = atof(pch);
  		if (countpart == 34) navdata.rotX = atof(pch);
  		if (countpart == 35) navdata.rotY = atof(pch);
  		if (countpart == 36) navdata.rotZ = atof(pch);
  		if (countpart == 37) navdata.altd = atof(pch);
	        if (countpart == 38) navdata.motor1 = atof(pch);
		if (countpart == 39) navdata.motor2 = atof(pch);
		if (countpart == 40) navdata.motor3 = atof(pch);
		if (countpart == 41) navdata.motor4 = atof(pch);
		countpart ++;
    		pch = strtok (NULL, " ,");
  	}
	
	//printf(" %.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%f,%f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f ", navdata.tm,navdata.pitch_w,navdata.pitch_a,navdata.pitch,navdata.roll_w,navdata.roll_a,navdata.roll,navdata.yaw_w,navdata.yaw_m,navdata.yaw,  navdata.dt,navdata.ts,navdata.hraw,navdata.h_meas,navdata.ax,navdata.ay,navdata.az,navdata.wx,navdata.wy,navdata.wz,navdata.magX,navdata.magY,navdata.magZ,navdata.pressure,navdata.rotX,navdata.rotY,navdata.rotZ,navdata.altd,navdata.motor1,navdata.motor2,navdata.motor3,navdata.motor4);
 	navdata.header.stamp = ros::Time::now(); 	
 	navdata_pub.publish(navdata);	
        //printf(" %.3f", navdata.tm);
   	ros::spinOnce();
  	loop_rate.sleep();        
  }

  udpClient_Close(&udp);
  return 0;
}
