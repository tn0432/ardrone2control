#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "udp.h"
#include "type.h"
#include "util.h"
#include "geometry_msgs/Twist.h"
#include "geometry_msgs/PoseStamped.h"
#include "std_msgs/Char.h"
#include <pthread.h>
#include <ctype.h>    /* For tolower() function */
#include <math.h>
#include <ros/ros.h>
#include <tf/transform_listener.h>

class CommunicationToDrone
{
public:
  
  CommunicationToDrone()
  {
    //Topic you want to subscribe
    sub_ = n_.subscribe("Robot_1/pose", 1, &CommunicationToDrone::Pose_Callback, this);
    mannualsub_ = n_.subscribe("cmd_vel", 1, &CommunicationToDrone::cmd_vel_Callback, this);

    //if(udpClient_Init(&udp,"192.168.43.100",7777)) diep("udpClient_Init");
    if(udpClient_Init(&udp,"192.168.1.1",7777)) diep("udpClient_Init");

    current_time = ros::Time::now();
    last_time = ros::Time::now();
  }

  void cmd_vel_Callback(const geometry_msgs::Twist& msg)
  {
    //n_.getParam("/mode", modeset);
    //printf("mode [%s] \n",modeset.c_str()); 
    if (n_.getParam("/mode", modeset))
    {
      //ROS_INFO("Got param: %s", modeset.c_str());
    }
    else
    {
      ROS_ERROR("Failed to get param 'my_param'");
    }
    vztglobal = msg.linear.z;

    if (modeset == "0") // Manual Control
    { 
        ROS_INFO("mode 0");
    	// udp_struct udp;
    	int msglen;
   	char buf[2048];
    	float ext = 0.0;
    	float eyt = 0.0;
    	float ezt = 0.0;
    	float evxt = msg.linear.x;
    	float evyt = msg.linear.y;
    	float evzt = msg.linear.z;
    	float axt = 0.0;
    	float ayt = 0.0;
    	float azt = 0.0;
    	float yawratet = msg.angular.z;
    	//printf("Pose x [%.5f] \n",QuadrotorPose.position.x);
  
    	//msglen=sprintf(buf, (char*)" %.3f,%.3f,%.3f,%.3f ", updown,frontback,leftright,yawrotate);
    	msglen=sprintf(buf, (char*)" %.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%s ",ext,eyt,ezt,evxt,evyt,evzt,axt,ayt,azt,yawratet,modeset.c_str());
    	if (udpClient_Send(&udp, buf, msglen)) diep("send");
    	//printf(" %.3f,%.3f,%.3f,%.3f \n",frontback,leftright,yawrotate,updown);   
    	//printf("xt %.3f, yt %.3f, zt %.3f, vxt %.3f, vyt %.3f, vzt %.3f, axt %.3f, ayt %.3f, azt %.3f, rollt %.3f, pitcht %.3f, yawt %.3f, rollratet %.3f, pitchratet %.3f, yawratet %.3f \n", xt,yt,zt,vxt,vyt,vzt,axt,ayt,azt,rollt,pitcht,yawt,rollratet,pitchratet,yawratet);
  	}
  }

  void Pose_Callback(const geometry_msgs::PoseStamped& msgPose) // Trajectory Control
  {
    //n_.getParam("/mode", modeset);
    //printf("mode [%s] \n",modeset.c_str()); 
    if (n_.getParam("/mode", modeset))
    {
      //ROS_INFO("Got param: %s", modeset.c_str());
    }
    else
    {
      ROS_ERROR("Failed to get param 'my_param'");
    }

    if (modeset == "1")
    { 
        //ROS_INFO("mode 1");
   	tf::StampedTransform transform;
    	try{
      		listener.lookupTransform("/Robot_1/base_link","/goal",   
                               ros::Time(0), transform);
    	}
    	catch (tf::TransformException ex){
      		ROS_ERROR("%s",ex.what());
      		ros::Duration(1.0).sleep();
    	}
    
   	
    	// udp_struct udp;
    	int msglen;
    	char buf[2048];
    	float ex = transform.getOrigin().x(); //msgPose.Position.x; w.r.t robot frame
    	float ey = transform.getOrigin().y(); //msgPose.Position.y;
    	float ez = transform.getOrigin().z();
        //float ezt = 0;
        current_time = ros::Time::now();
        float dt = (current_time - last_time).toSec();
	geometry_msgs::Twist twistframes;
        try{
 		listener.lookupTwist("/Robot_1/base_link","/goal",   
                               ros::Time(0), ros::Duration(dt), twistframes);
    	}
    	catch (tf::TransformException ex){
      		ROS_ERROR("%s",ex.what());
      		ros::Duration(1.0).sleep();
    	}
       	last_time = current_time;
    	float evx = twistframes.linear.x; 	
    	float evy = twistframes.linear.y;
    	float evz = twistframes.linear.z;
        //float evzt = vztglobal;
        //ROS_INFO("evt[%.3f,%.3f,%.3f] dt%.3f,", evx,evy,evz,dt); 
         
        
	//float vzt = vztglobal; 
    	float axt = 0.0;
    	float ayt = 0.0;
    	float azt = 0.0;
    	float yawratet = 1 * tf::getYaw(transform.getRotation());

        //ROS_INFO("ert[%.3f,%.3f,%.3f,%.3f]", ex,ey,ez,yawratet);

   	 
        float F[3] = {0.0,0.0,0.0}; 
  
        F[1] =  0.7 * ex + 0.005 * evx + axt; //0.005
	F[2] =  -0.7 * ey - 0.005 * evy + ayt;
	F[3] = 0.72 + (0.1 * ez) + (0.000 * evz) + azt; //4.03191;.

	if(F[1]>=0.2)
	{
		F[1] = 0.2;
	}
	if(F[1]<=-0.2)
	{
		F[1] = -0.2;
	}
	if(F[2]>=0.2)
	{
		F[2] = 0.2;
	}
	if(F[2]<=-0.2)
	{
		F[2] = -0.2;
	}

	if(F[3]>=0.95)
	{
		F[3] = 0.95;
	}
	if(yawratet>=0.1)
	{
		yawratet = 0.1;
	}
	if(yawratet<=-0.1)
	{
		yawratet = -0.1;
	}

	ROS_INFO("dt%.3f_F[%.5f,%.5f,%.5f]_ert[%.3f,%.3f,%.3f,%.3f]_evt[%.3f,%.3f,%.3f]",dt,F[1],F[2],F[3],ex,ey,ez,yawratet,evx,evy,evz);

    	//printf("Pose x [%.5f] \n",QuadrotorPose.position.x);
        //printf("Pose x [%.5f] \n",xt);
        //printf("Pose y [%.5f] \n",yt);
        //printf("Pose vzt [%.5f] \n",vzt);
        //ROS_INFO("Pose yaw [%.5f] \n",yawt);
	//msglen=sprintf(buf, (char*)" %.3f,%.3f,%.3f,%.3f ", updown,frontback,leftright,yawrotate);
    	msglen=sprintf(buf, (char*)" %.3f,%.3f,%.3f,%.3f,%s ",F[3],F[1],F[2],yawratet,modeset.c_str());
    	if (udpClient_Send(&udp, buf, msglen)) diep("send");
    	//printf(" %.3f,%.3f,%.3f,%.3f \n",frontback,leftright,yawrotate,updown);
      	//printf("xt %.3f, yt %.3f, zt %.3f, vxt %.3f, vyt %.3f, vzt %.3f, axt %.3f, ayt %.3f, azt %.3f, rollt %.3f, pitcht %.3f, yawt %.3f, rollratet %.3f, pitchratet %.3f, yawratet %.3f \n", xt,yt,zt,vxt,vyt,vzt,axt,ayt,azt,rollt,pitcht,yawt,rollratet,pitchratet,yawratet);
	}
  }

  void diep(const char *s)
  {
    perror(s);
    exit(1);
  }

private:
  udp_struct udp;
  float vztglobal;
  float pre_ext;
  float pre_eyt;
  float pre_ezt;
  ros::Time current_time;
  ros::Time last_time;

  ros::NodeHandle n_; 
  ros::Publisher pub_;
  ros::Subscriber sub_;
  ros::Subscriber mannualsub_;
  //ros::NodeHandle nh;
  std::string modeset;
  //modeset = "0";
  //geometry_msgs::Pose QuadrotorPose;
  //
  tf::TransformListener listener;

};//End of class SubscribeAndPublish

int main(int argc, char **argv)
{

  //Initiate ROS
  ros::init(argc, argv, "ardrone_ground_node_trajectorycontrol");
  // Broadcast a simple log message
  ROS_INFO_STREAM("Start ROS node!");

  //Create an object of class CommunicationToDrone that will take care of everything
  CommunicationToDrone CommunicationObject;
  ros::Rate loop_rate(200);

  ros::spin();

  return 0;
}

  //udpClient_Close(&udp);


