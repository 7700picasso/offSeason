/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       student                                                   */
/*    Created:      4/3/2025, 6:33:40 PM                                      */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "vex.h"

using namespace vex;

// A global instance of competition
competition Competition;
brain Brain;
motor LB(PORT11, ratio6_1, true);
motor RB(PORT20, ratio6_1, false);
motor LM(PORT6, ratio6_1, true);
motor RM(PORT10, ratio6_1, false);
inertial imu(PORT7);

// define your global instances of motors and other devices here
float dia = 3.25;
float gearRatio = 0.6;
float WC = dia * M_PI;
float distanceTarget = 0;
bool driveTaskActive = false;
float turnTarget = 0;
bool turnTaskActive = false;

/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */

float getDistanceInches(){
  float LMRevs = LM.position(rev) * gearRatio;
  float RMRevs = RM.position(rev) * gearRatio;
  float avgRevs = (LMRevs + RMRevs) / 2;
  return avgRevs * WC;
}

void resetDrive(){
  LM.resetPosition();
  RM.resetPosition();
  LB.resetPosition();
  RB.resetPosition();
  imu.resetRotation();
  driveTaskActive = false;
}

int driveTask(){
  while(true){
    if(driveTaskActive){
      float current = getDistanceInches();
      float error = distanceTarget - current;
      float kp = 3.75;
      float speed = error * kp;

      LM.spin(fwd, speed, pct);
      RM.spin(fwd, speed, pct);
      LB.spin(fwd, speed, pct);
      RB.spin(fwd, speed, pct);

      if(fabs(error) < 0.8){
        LM.stop(brake);
        LB.stop(brake);
        RM.stop(brake);
        RB.stop(brake);
        driveTaskActive = false;
      }
    }
   wait(20, msec);
  }
  return 0;
}

void driveDistance(float inches){
  resetDrive();
  distanceTarget = inches;
  driveTaskActive = true;
  while(driveTaskActive){
    wait(10, msec);
  }
}

int turnTask(){
  while(true){
    if(turnTaskActive){
      float theta = imu.rotation(deg);
      float error = turnTarget - theta;
      float kp = 0.7;
      float speed = error * kp;

      LM.spin(fwd, speed, pct);
      RM.spin(fwd, -speed, pct);
      LB.spin(fwd, speed, pct);
      RB.spin(fwd, -speed, pct);
      
      if(fabs(error) < 0.2){
        LM.stop(brake);
        LB.stop(brake);
        RM.stop(brake);
        RB.stop(brake);
        turnTaskActive = false;
      }
    }
    wait(20, msec);
  }
  return 0;
}

void turnDistance(float degrees){
  resetDrive();
  turnTarget = degrees;
  turnTaskActive = true;
  while(turnTaskActive){
    wait(10, msec);
  }
}

/*---------------------------------------------------------------------------*/

void pre_auton(void) {
  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...
  resetDrive();
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
  
  // task dc (driveTask);
  // wait(500, msec);
  // driveDistance(24);

  // dc.stop();
  // Brain.Screen.printAt(10, 50, "final distance: %0.1f", getDistanceInches());

  task tc (turnTask);
  wait(500, msec);
  turnDistance(90);

  tc.stop();
  Brain.Screen.printAt(10, 50, "final degrees: %0.1f", imu.rotation(deg));

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
