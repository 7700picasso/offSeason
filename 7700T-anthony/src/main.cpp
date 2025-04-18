/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       student                                                   */
/*    Created:      4/3/2025, 6:31:37 PM                                      */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "vex.h"

using namespace vex;

// A global instance of competition
competition Competition;

// define your global instances of motors and other devices here
brain Brain;
motor LF(PORT8, ratio18_1, false);
motor RF(PORT10, ratio18_1, true);
motor LB(PORT18, ratio6_1, false);
motor RB(PORT9, ratio6_1, true);
inertial gyro1(PORT1);

float dia = 3.75;
float gearRatio = 0.6;
float wheelCircumference = dia * M_PI;

float distanceTarget = 0;
bool driveTaskActivate = false;

float desireheading = 0;
float angleTarget = 0;
bool turnTaskActive = false; 
float kp = 0.5;
/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */


float getDistanceInches(){
  float LFrevs = LF.position(rev) * gearRatio;
  float RFrevs = RF.position(rev) * gearRatio;
  float avgRevs = (LFrevs + RFrevs)/2;
  return avgRevs * wheelCircumference;
}

void resetDrive(){
  LF.resetPosition();
  RF.resetPosition();
  gyro1.resetRotation();
  driveTaskActivate = false;
  turnTaskActive = false;

}

int driveTask() {
  Brain.Screen.clearScreen(); 
  Brain.Screen.printAt(10, 50, "task entered"); 
while(true){ 
    Brain.Screen.printAt(10, 50, "task started"); 

  if (driveTaskActivate){
      Brain.Screen.printAt(10, 50, "task activated"); 

  float currentDistance = getDistanceInches();
  float error = distanceTarget - currentDistance;
  float kp = 0.9;
  float speed = error * kp;

  LF.spin(forward, speed, pct);
  RF.spin(forward, speed, pct);
  LB.spin(forward, speed, pct);
  RB.spin(forward, speed, pct);
  Brain.Screen.printAt(50,150, "task running" ); 

if (fabs(error) < 0.67) {
  LF.stop(brake);
  RF.stop(brake);
  LB.stop(brake);
  RB.stop(brake);
  driveTaskActivate = false;
  Brain.Screen.printAt(50,150, "task ended" ); 
}
  }
wait(20, msec);
}

return 0;
}

void driveDistance(float inches) {
  resetDrive();
  distanceTarget = inches;
  driveTaskActivate = true;
  while (driveTaskActivate){
    wait(10,msec);
  }
}

int turnTask () {
  Brain.Screen.printAt(10,50,"TASK INITIATED");
  while(true) {
    Brain.Screen.printAt(10,70,"TASK STARTED");
    if (turnTaskActive){
      Brain.Screen.printAt(10,90,"ROBOT TURNING");
      float currentAngle = gyro1.rotation();
      float error = angleTarget - currentAngle;
      Brain.Screen.printAt(10,110,"ANGLE = %0.1f", currentAngle);
      Brain.Screen.printAt(10,130,"eroror = %0.1f", error);
      if (fabs (error)>1.5) {
        float speed = kp*error;
        LF.spin(forward, speed, pct);
        RF.spin(forward, -speed, pct);
        LB.spin(forward, speed, pct);
        RB.spin(forward, -speed, pct);

      }

      else {
        LF.stop(brake);
        RF.stop(brake);
        LB.stop(brake);
        RB.stop(brake);
        //turnTaskActive = false;
      }


    }
    wait(20, msec);

  }
  return 0;
}

void turnToAngle (float degrees){
  desireheading = degrees;
  angleTarget = degrees;
  turnTaskActive = true;
  while (turnTaskActive) {
    wait(10,msec);
  }
}
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the V5 has been powered on and        */
/*  not every time that the robot is disabled.                               */
/*---------------------------------------------------------------------------*/


void pre_auton(void) {
  resetDrive();
  while(gyro1.isCalibrating()){
    wait(200, msec);
  }
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

  task dc (driveTask);
  task tc (turnTask);
  wait(500, msec);


  driveDistance(24);
  turnToAngle(90);


  tc.stop();
  dc.stop();
  Brain.Screen.printAt(50,50, "Fina Dist: %0.1f", getDistanceInches());
  Brain.Screen.printAt(50,50, "Fina Dist: %0.1f", gyro1.rotation());








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
