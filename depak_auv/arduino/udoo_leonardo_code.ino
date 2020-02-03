// Project depak
// Thruster control and sensor data acquisition code to be deployed
// on on-board arduino leonardo of the Udoo board
// Functions:
// - accept Thruster setpoints and produce appropriate RPM
// - acquire data from
//    - fluid flow sensor
//    - turbidity sensor
//    - temperature sensor

// Initial author: Osaid Zahid
// Comments: This code could use some serious revision :)
// Need to move to Matrix operations



#define USE_USBCON 

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include <WProgram.h>
#endif

#include <Servo.h>

#include <ros.h>
#include <std_msgs/UInt16.h>
#include <std_msgs/String.h>
#include <std_msgs/Int32.h>
#include <std_msgs/Empty.h>
#include <geometry_msgs/Twist.h>

ros::NodeHandle  nh;
//Deadband 1520 - 1480

const int minFwd = 1500;
const int maxFwd = 1700; //2000
const int maxRev = 1300;
const int minRev = 1420; //1100
const int Stop = 1500;

Servo M1_F; //Front
Servo M2_F;
Servo M1_B; //Back
Servo M2_B;
Servo M1_U;
Servo M2_D;

std_msgs::Int32 str_msg;
ros::Publisher chatter("chatter", &str_msg);

// Arduino 'map' funtion for floating point
double fmap (double toMap, double in_min, double in_max, double out_min, double out_max) {
  return (toMap - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


//F:-1,-1,1,1 M:1,2,3,4,

void Fwd_back(int Rev, int Fwd, bool dir) {

  if (dir) {

    M1_F.writeMicroseconds(Rev);
    M2_F.writeMicroseconds(Rev);
    M1_B.writeMicroseconds(Fwd);
    M2_B.writeMicroseconds(Fwd);

  }

  else {

    M1_F.writeMicroseconds(Fwd);
    M2_F.writeMicroseconds(Fwd);
    M1_B.writeMicroseconds(Rev);
    M2_B.writeMicroseconds(Rev);

  }


}

void Left_Right(int Rev, int Fwd, bool dir) {

  if (dir) {

    M1_F.writeMicroseconds(Fwd);
    M2_F.writeMicroseconds(Rev);
    M1_B.writeMicroseconds(Fwd);
    M2_B.writeMicroseconds(Rev);

  }

  else {

    M1_F.writeMicroseconds(Rev);
    M2_F.writeMicroseconds(Fwd);
    M1_B.writeMicroseconds(Rev);
    M2_B.writeMicroseconds(Fwd);

  }

}

void yaw(int Rev, int Fwd, bool dir) {

  if (dir) {

    M1_F.writeMicroseconds(Fwd);
    M2_F.writeMicroseconds(Rev);
    M1_B.writeMicroseconds(Rev);
    M2_B.writeMicroseconds(Fwd);

  }

  else {

    M1_F.writeMicroseconds(Rev);
    M2_F.writeMicroseconds(Fwd);
    M1_B.writeMicroseconds(Fwd);
    M2_B.writeMicroseconds(Rev);

  }

}

void ascend_descend(int Rev, int Fwd, bool dir) {

  if (dir) {

    M1_U.writeMicroseconds(Fwd);
    M2_D.writeMicroseconds(Fwd);
  }

  else {
    M1_U.writeMicroseconds(Rev);
    M2_D.writeMicroseconds(Rev);
  }
}

//Forward, Reverse,
void driveCallback ( const geometry_msgs::Twist&  twistMsg )
{


  int Fwd, Rev;


  if (twistMsg.linear.x > 0) {
    
    Fwd = (int)fmap(twistMsg.linear.x, 0, 1, minFwd, maxFwd);
    Rev = (int)fmap(twistMsg.linear.x, 0, 1, minRev, maxRev);
    Fwd_back(Rev, Fwd, 1);

  } else {

    Fwd = (int)fmap(twistMsg.linear.x, 0, 1, minFwd, maxFwd);
    Rev = (int)fmap(twistMsg.linear.x, 0, 1, minRev, maxRev);
    Fwd_back(Rev, Fwd, 0);
  }
/*
  if (twistMsg.linear.y > 0) {

    Fwd = (int)fmap(twistMsg.linear.y, 0, 1, minFwd, maxFwd);
    Rev = (int)fmap(twistMsg.linear.y, 0, 1, minRev, maxRev);
    Fwd_back(Rev, Fwd, 1);

  } else {

    Fwd = (int)fmap(twistMsg.linear.y, 0, 1, minFwd, maxFwd);
    Rev = (int)fmap(twistMsg.linear.y, 0, 1, minRev, maxRev);
    Fwd_back(Rev, Fwd, 0);
  }
  if (twistMsg.linear.z > 0) {

    Fwd = (int)fmap(twistMsg.linear.z, 0, 1, minFwd, maxFwd);
    Rev = (int)fmap(twistMsg.linear.z, 0, 1, minRev, maxRev);
    ascend_descend(Rev, Fwd, 0);

  } else {

    Fwd = (int)fmap(twistMsg.linear.z, 0, 1, minFwd, maxFwd);
    Rev = (int)fmap(twistMsg.linear.z, 0, 1, minRev, maxRev);
    ascend_descend(Rev, Fwd, 0);

  }

  if (twistMsg.angular.z > 0) {

    Fwd = (int)fmap(twistMsg.angular.z, 0, 1, minFwd, maxFwd);
    Rev = (int)fmap(twistMsg.angular.z, 0, 1, minRev, maxRev);
    yaw(Rev, Fwd, 0);

  } else {

    Fwd = (int)fmap(twistMsg.angular.z, 0, 1, minFwd, maxFwd);
    Rev = (int)fmap(twistMsg.angular.z, 0, 1, minRev, maxRev);
    yaw(Rev, Fwd, 0);

  }



  // The following could be useful for debugging

 M1_F.writeMicroseconds(1500);
  M2_F.writeMicroseconds(1500);
  M1_B.writeMicroseconds(1500);
  M2_B.writeMicroseconds(1500);
  M1_U.writeMicroseconds(1500);
  M2_D.writeMicroseconds(1500);
*/
  str_msg.data= Fwd ;
  chatter.publish(&str_msg);
  // Stop all Thrusters
  

  //digitalWrite(13, HIGH-digitalRead(13));  //toggle led

}

ros::Subscriber<geometry_msgs::Twist> driveSubscriber("/cmd_vel", &driveCallback) ;

void setup() {
  
  //nh.getHardware->setBaud(115200);
  nh.initNode();

  // This can be useful for debugging purposes
  nh.advertise(chatter);

  // Subscribe to the steering and throttle messages
  nh.subscribe(driveSubscriber) ;

  // Attach the servos to actual pins
  M1_F.attach(13);
  M2_F.attach(11);
  M1_B.attach(3);
  M2_B.attach(5);
  M1_U.attach(6);
  M2_D.attach(6);

  // Stop all Thrusters
  M1_F.writeMicroseconds(Stop);
  M2_F.writeMicroseconds(Stop);
  M1_B.writeMicroseconds(Stop);
  M2_B.writeMicroseconds(Stop);
  M1_U.writeMicroseconds(Stop);
  M2_D.writeMicroseconds(Stop);


  delay(1000);

}

void loop() {
  nh.spinOnce();
  delay(1);
}
