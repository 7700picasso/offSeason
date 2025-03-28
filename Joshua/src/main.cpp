/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       student                                                   */
/*    Created:      3/27/2025, 7:05:00 PM                                     */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "vex.h"

using namespace vex;

// A global instance of competition
competition Competition;

brain Brain;
controller Controller;

motor LF (PORT3, ratio6_1, true);
motor LM (PORT5, ratio6_1, false);
motor LB (PORT7, ratio6_1, true);

motor RF (PORT12, ratio6_1, false);
motor RM (PORT19, ratio6_1, true);
motor RB (PORT13, ratio6_1, false);

inertial IMU (PORT20);

// define your global instances of motors and other devices here
float dia = 3.25;
float WC = dia * M_PI;

float distanceTarget= 0;
float angleTarget = 0;
bool driveTaskActive = false;
bool driveTurnActive = false;

/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
float getDistanceInches () {
  float leftRevs = LF.position(rev);
  float rightRevs = RF.position(rev);
  float avgRevs =  (leftRevs + rightRevs) / 2.0;
  return avgRevs = WC;

}

void resetDirve() {
  LF.resetPosition();
  RF.resetPosition();
  IMU.resetRotation();
  bool driveTaskActive = false;
bool driveTurnActive = false;

int driveTask(){
while (true) {
  float currentDistance = getDistanceInches();
  float error = targetdistance - currentDistance;
  float speed = error * 0.5;

  RM.spin(forward, speed, percent);
  LM.spin (forward, speed, percent);
  LB.spin(forward, speed, percent);
  RB.spin(forward, speed, percent);
  LF.spin(forward, speed, percent);
  RF.spin(forward, speed, percent);

  if ((fabs(error) < 0.5)){
    RM.stop(brake);
    LM.stop(brake);
    LF.stop(brake);
    RF.stop(brake);
    RB.stop(brake);
    LB.stop(brake);
    driveTaskActive = false
  }
}
}
return 0;
}
int turnTask (){
  while(true){
    float currentAngle = IMU.rotation();
    float error = angleTarget - currentAngle;

    RM.spin(reverse, speed, percent);
    LM.spin(reverse, speed, percent);
    LB.spin(reverse, speed, percent);
    RB.spin(reverse, speed, percent);
    LF.spin(reverse, speed, percent);
    RF.spin(reverse, speed, percent);
    if(fabs(error) > 2);
    RM.stop(brake);
    LM.stop(brake);
    turnTaskActive = (false);
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
