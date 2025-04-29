/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       student                                                   */
/*    Created:      4/28/2025, 5:23:04 PM                                     */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "vex.h"

using namespace vex;

// A global instance of competition
competition Competition;
vex::brain       Brain;
motor LB(PORT8, ratio6_1, false);
motor LF(PORT7, ratio6_1, false);
motor RB(PORT1, ratio6_1, true);
motor RF(PORT2, ratio6_1, true);
inertial Gyro(PORT13);
// define your global instances of motors and other devices here

float dia = 3.25;
float gearRatio = 0.75;
float WC = dia * M_PI;

float distanceTarget = 0;
float angleTarget = 0;

bool driveTaskActive = false;
bool turnTaskActive = false;

float desiredHeading = 0;

float getDistanceInches(){
    float leftDegrees =  LF.position(deg);
    float rightDegrees =  RF.position(deg);

    float leftMotorRevs = leftDegrees / 360.0;
    float rightMotorRevs = rightDegrees / 360.0;

    float leftWheelRevs = leftMotorRevs * gearRatio;
    float rightWheelRevs = rightMotorRevs * gearRatio;

    return ((leftWheelRevs + rightWheelRevs / 2.0)) * (dia *3.14159);
}

void resetDrive(){
    LF.resetPosition();
    RF.resetPosition();
    turnTaskActive = false;
    driveTaskActive = false;
}
int driveTask(){
    Brain.Screen.printAt(10, 30, "drive task initiated");

    while (true){
        Brain.Screen.printAt(10, 60, "drive task entered");

        if(driveTaskActive){
            Brain.Screen.clearScreen();
            Brain.Screen.printAt(10, 80, "driving");

            float current =  ((LF.position(rev) + LF.position(rev))/2) * M_PI*dia*gearRatio;
            float error = distanceTarget - current;
            Brain.Screen.clearScreen();
            Brain.Screen.printAt(10, 100, "Cur: %.1f Err: %.1f", current, error);

            float speed = error * 4.75;
            speed = fmax(fmin(speed, 90), -90);

            LF.spin(fwd, speed, pct);
            RF.spin(fwd, speed, pct);
            LB.spin(fwd, speed, pct);
            RB.spin(fwd, speed, pct);

            if(fabs(error) < 0.2){
            LF.stop();
            RF.stop();
            LB.stop();
            RB.stop();
             driveTaskActive = false;
              Brain.Screen.printAt(10, 120, "reached target.");
            }
            
        }
        wait(20, msec);
    }
    return 0;
}

void driveDistance(double inches){
  resetDrive();
  distanceTarget = inches;
  driveTaskActive = true;
  while (driveTaskActive){
    wait(10, msec);
  }
}

// define your global instances of motors and other devices here

/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the V5 has been powered on and        */
/*  not every time that the robot is disabled.                               */
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
  task dc(driveTask);
 // wait(500, msec);

  driveDistance(20);
  dc.stop();
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
