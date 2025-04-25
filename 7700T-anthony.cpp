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
motor LF(PORT4, ratio6_1, true);
motor RF(PORT19, ratio6_1, false);
motor LB(PORT2, ratio6_1, true);
motor RB(PORT21, ratio6_1, false);
inertial gyro1(PORT10);

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

struct robot{
  float x = 0;
  float y = 0;
  float heading = 0;
}myRobot;

void moveForward(float targetInches){
  float distance = 0;
  float error = distanceTarget - distance;
  float Kp = 1.67;
  float speed = error * Kp;
  LF.resetPosition;
  RF.resetPosition;
  while (fabs(error) > 0.67){
    distance = (LF.position(deg) + RF.position(deg))/2.0 * WC * gearRatio;
    error = distanceTarget - distance;
    speed = error * Kp;
    LF.spin(forward, speed, pct);
    LB.spin(forward, speed, pct);
    RF.spin(forward, speed, pct);
    RB.spin(forward, speed, pct);
  }
  LF.stop();
  LB.stop();
  RF.stop();
  RB.stop();

  MyRobot.x += targetInches * cos (myRobot.heading*(3.14159265358979323846264338327950288)/180);
  MyRobot.y += targetInches * sin (mrRobot.heading*(3.14159265358979323846264338327950288)/180);
}

void turnToTheAngle (float targetDegrees){
  float Kp = 0.8;
  float tolerance = 2.0;

  while(true){
    float error = targetDegrees - gyro1.rotation(deg);

    while (error > 180) error -= 360;
    while (error < -180) error += 360;

    if (fabs(error) < tolerance) break;
  }
}

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
  float kp = 1.67;
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
        turnTaskActive = false;
        Brain.Screen.printAt(50,100,"Turned");
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
  wait(5, sec);
  turnToAngle(180);


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
