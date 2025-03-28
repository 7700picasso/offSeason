/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       mia is yawning                                            */
/*    Created:      3/27/2025, 7:06:01 PM                                     */
/*    Description:  sigma boy                                                 */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "vex.h"

using namespace vex;

// A global instance of competition
competition Competition;
brain Brain;

// define your global instances of motors and other devices here
motor LM = motor(PORT6, ratio6_1, true);
motor RM = motor(PORT4, ratio6_1, false);
motor LB = motor(PORT11, ratio6_1, true);
motor RB = motor(PORT20, ratio6_1, false);
inertial imu (PORT1);

// variables
float dia = 3.25;
float WC = dia * M_PI;

float distanceTarget = 0;
float angleTarget = 0;

bool driveTaskActive = false;
bool turnTaskActive = false;

/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */

float getDistanceInches() {
  float leftRevs = LB.position(rev);
  float rightRevs = RB.position(rev);
  float avgRevs = (leftRevs+rightRevs)/2.0;
  return avgRevs = WC;
}

void resetDrive() {
  LB.resetPosition();
  RB.resetPosition();
  imu.resetRotation();
  driveTaskActive = false;
  turnTaskActive = false;
}

void stopDrive() {
  LB.stop(brake);
  LM.stop(brake);
  RM.stop(brake);
  RB.stop(brake);
}

int driveTask() {
  while(true){
    if(driveTaskActive) {
      float currentDistance = getDistanceInches();
      float error = distanceTarget - currentDistance;
      float speed = error * 0.8; // kp = 0.8
      LB.spin(fwd, speed, pct);
      RB.spin(fwd, speed, pct);
      LM.spin(fwd, speed, pct);
      RM.spin(fwd, speed, pct);
      if(fabs(error) < 0.5){
        stopDrive();
        driveTaskActive = false;
      }
    }
    return 0;
  }
}

int turnTask() {
  while(true){
    if(turnTaskActive) {
      float currentAngle = imu.rotation(deg);
      float error = angleTarget - currentAngle;
      float speed = error * 0.8; // kp = 0.8
      LB.spin(fwd, speed, pct);
      RB.spin(reverse, speed, pct);
      LM.spin(fwd, speed, pct);
      RM.spin(reverse, speed, pct);
      if(fabs(error) < 2.0){
        stopDrive();
        turnTaskActive = false;
      }
    }
    return 0;
  }
}

/*---------------------------------------------------------------------------*/

void pre_auton(void) {

  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...



}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              Autonomous Task                              */
/*                                                                           */
/*  This task is used to control your robot during the autonomous phase of   */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void autonomous(void) {
  // ..........................................................................
  // Insert autonomous user code here.
  // ..........................................................................
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              User Control Task                            */
/*                                                                           */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void usercontrol(void) {
  // User control code here, inside the loop
  while (1) {
    // This is the main execution loop for the user control program.
    // Each time through the loop your program should update motor + servo
    // values based on feedback from the joysticks.

    // ........................................................................
    // Insert user code here. This is where you use the joystick values to
    // update your motors, etc.
    // ........................................................................

    wait(20, msec); // Sleep the task for a short amount of time to
                    // prevent wasted resources.
  }
}

//
// Main will set up the competition functions and callbacks.
//
int main() {
  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  // Run the pre-autonomous function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}
