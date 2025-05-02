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

motor LF (PORT2, ratio6_1, true);
motor LB (PORT12, ratio6_1, false);
motor RF (PORT5, ratio6_1, false);
motor RB (PORT11, ratio6_1, false);

inertial IMU (PORT20);

// define your global instances of motors and other devices here
float dia = 3.25;
float WC = dia * M_PI;
float gearRatio = 0.4;

float distanceTarget= 0;
float angleTarget = 0;
bool driveTaskActive = false;
bool driveTurnActive = false;
bool turnTaskActive = 0;
float desiredHeading = 0;

/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */

struct robot{
  float x = 0;
  float y= 0;
  float heading = 0;
  Robot(double x_init = 0, double x)
}myRobot;

void moveForward(float targetInches) {
  float distance =0;
  float error = targetInches - distance;
  float kp = 3.0;
  float speed = error * kp;
  LF.resetPosition();
  LB.resetPosition();
  RF.resetPosition();
  RB.resetPosition();

  while(fabs (error)>0.5){


    (LF.position(deg)+RF.position(deg)) / 2.0 * WC *gearRatio;
    (RF.position(deg)+LF.position(deg)) / 2.0 * WC *gearRatio;
    (RB.position(deg)+LB.position(deg)) / 2.0 * WC *gearRatio;
    (LB.position(deg)+RB.position(deg)) / 2.0 * WC *gearRatio;
    error = targetInches - distance;
    speed = error * kp;
    LF.spin(fwd, speed, pct); 
    LB.spin(fwd, speed, pct); 
    RB.spin(fwd, speed, pct); 
    RF.spin(fwd, speed, pct); 
  }
  LF.stop();
  LB.stop();
  RF.stop();
  RB.stop();

  myRobot.x += targetInches *cos (myRobot.heading * (3.14/180));
  myRobot.y += targetInches *sin (myRobot.heading * (3.14/180));
}

void turnToAngle(double targetDegrees) {
  double kp = 0.8;
  double tolerance = 2.0;

  while (true){
    double currentHeading = IMU.heading();
    
    if (currentHeading > 180) currentHeading -= 360;
    Brain.Screen.printAt(10,115,Current: %.1f, Target: %.1f, Error: %.1f);
    while (true){
      double error = targetDegrees - currentheading();
  
      while (error > 180) error -= 360;
      while (error <-180) error += 360;
  
      if (fabs(error) < tolerancee) break;
      
    }
    RF.spin(forward, speed, pct);
    LF.spin(forward, speed, pct);
    LB.spin(forward, speed, pct);
    RB.spin(forward, speed, pct);

    
  }
}

void goToPoint(double targetX, double tagetY){
  double dx = targetX-myRobot.x;
  double dy-myRobot.y;
}

doubletargetAngle = atan2(dx, dy) * (180.0/3.14);

if (fabs(trgetAngle - myRobt.angle)>5.0){





  
  turnToAngle(targetAngle);
}

doubledistance = sqrt(dx*dx + dy*dy);
moveForward(disance);

Controller.rumble(".-.")


void turnToTheAngle (double targetDegrees){
  double kp = 0.8;
  double tolerate = 2.0;

  while (true){
    double error = targetDegrees - currentheading();

    while (error > 180) error -= 360;
    while (error <-180) error += 360;

    if (fabs(error) < tolerancee) break;
    
  }
}



float getDistanceInches () {
  float leftRevs = LF.position(rev);
  float rightRevs = RF.position(rev);
  float avgRevs =  (leftRevs + rightRevs) / 2.0;
  return avgRevs * WC;

}

void resetDirve(){
  LF.resetPosition();
  RF.resetPosition();
  IMU.resetRotation();
  bool driveTaskActive = false;
bool driveTurnActive = false;
}
int driveTask() {

while (true) {
  if(driveTaskActive) {
  float currentDistance = getDistanceInches();
  float error = distanceTarget - currentDistance;
  float kp = 1.5;
  float speed = error * 0.5;

  LB.spin(forward, speed, percent);
  RB.spin(forward, speed, percent);
  LF.spin(forward, speed, percent);
  RF.spin(forward, speed, percent);

  if ((fabs(error) < 0.5)){
    LF.stop(brake);
    RF.stop(brake);
    RB.stop(brake);
    LB.stop(brake);
    driveTaskActive = false;
  
  }
}
wait(20, msec);
}
return 0;
}

void driveDistance(float inches){
  resetDirve();
  distanceTarget = inches;
  driveTaskActive = true;
  while (driveTaskActive){
    wait(10, msec);
  }
}
int turnTask(){
  while(true){
    if (turnTaskActive){
      float currentAngle = IMU.rotation();
      float error = desiredHeading - currentAngle;
      if (fabs (error)>1.5){
        float correction = error * 0.5;
        LB.spin(forward, correction, pct);
        LF.spin(forward, correction, pct);
        RB.spin(forward, correction, pct);
        RF.spin(forward, correction, pct);
      }
      else{
        LB.stop(brake);
        LF.stop(brake);
        RB.stop(brake);
        RF.stop(brake);
        turnTaskActive = false;
      }
    }
      wait(20,msec);
    }
    return 0;
}

/*---------------------------------------------------------------------------*/

void pre_auton(void) {
  /*resetDirve();
  while(IMU.isCalibrating()){
    wait(200, msec);
}


  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...
*/}

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

  goToPoint(72, 24);
  goToPoint(120,24);
  /*task dc (driveTask);
  task tc(turnTask);
  wait(500, msec);


  driveDistance(24);
  turnToAngle(90);
  wait(5, sec);
  turnToAngle(180);

  tc.stop();
  dc.stop();
  */
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
