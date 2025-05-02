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
controller Controller1;
motor LB(PORT19, ratio18_1, true); // green motors :(
motor RB(PORT12, ratio18_1, false);
motor LM(PORT20, ratio18_1, true);
motor RM(PORT11, ratio18_1, false);
inertial imu(PORT15);

// define your global instances of motors and other devices here
float dia = 3.25;
float gearRatio = 0.6;
float WC = dia * M_PI;

float distanceTarget = 0;
bool driveTaskActive = false;
float desiredHeading = 0;
float turnTarget = 0;
bool turnTaskActive = false;

/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */


void stopDrive(){
  LM.stop(brake);
  RM.stop(brake);
  LB.stop(brake);
  RB.stop(brake);
}

struct robot{
  float x = 0;
  float y = 0;
  float heading = 0;

  robot(double x_init = 0, double y_init = 0, double heading_init = 0)
  : x(x_init), y(y_init), heading(heading_init) {}
}myRobot;


void moveForward(float targetInches){
  float distance = 0;
  float error = targetInches - distance;
  float kp = 0.2;
  float speed = kp * error;
  LM.resetPosition();
  RM.resetPosition();
  while(fabs(error) > 0.5){
    distance = (RM.position(deg) + LM.position(deg)) / 2.0 * WC * gearRatio;
    error = targetInches - distance;
    speed = kp * error;
    LM.spin(fwd, speed, pct);
    RM.spin(fwd, speed, pct);
    LB.spin(fwd, speed, pct);
    RB.spin(fwd, speed, pct);
  }
  LM.stop();
  RM.stop();
  LB.stop();
  RB.stop();
  myRobot.x += targetInches * cos(myRobot.heading * (3.14/180));
  myRobot.y += targetInches * sin(myRobot.heading * (3.14/180));
}


void turnToAngle(double targetDegrees){
    double kp = 0.8;
    double tolerance = 2.0;
    while(true){
      double currentHeading = imu.heading();
      if(currentHeading > 180) currentHeading -= 360;
      Brain.Screen.printAt(10, 100, "Normalized Heading: %0.1f", currentHeading);

      // keep error between -180 and 180
      double error = targetDegrees - currentHeading;
      while(error > 180) error -= 360;
      while(error < -180) error += 360;
      Brain.Screen.printAt(10, 115, "Current: %0.1f, Target: %0.1f, Error: %0.1f", currentHeading, targetDegrees, error);

      if(fabs(error) < tolerance) break;
      double speed = error * kp;
      LM.spin(fwd, speed, pct);
      RM.spin(fwd, -speed, pct);
      LB.spin(fwd, speed, pct);
      RB.spin(fwd, -speed, pct);
      wait(20, msec);
  }
  LM.stop();
  RM.stop();
  LB.stop();
  RB.stop();
  myRobot.heading = targetDegrees;
}


void goToPoint(double targetX, double targetY){
  // 1. Fubd direction to target
  double dx = targetX - myRobot.x;
  double dy = targetY - myRobot.y;

  // 2. Calculate target angle (0 = East)
  double targetAngle = atan2(dy, dx) * (180.0/3.14);
  Brain.Screen.printAt(10, 160, "heading = ", targetAngle);

  // 3. Only turn if NOT already facing the target (+-5 tolerance)
  if(fabs(targetAngle - myRobot.heading) > 5.0){
    turnToAngle(targetAngle);
  }

  // 4. Move straight
  double distance = sqrt(dx * dx + dy * dy);
  moveForward(distance);

  // 5. WOOHOO!
  Brain.Screen.printAt(10, 80, "Reached (%0.1f, %0.1f)!", targetX, targetY);
  Controller1.rumble(".");
}


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
  // imu.resetRotation();
  driveTaskActive = false;
  turnTaskActive = false;
}


int driveTask(){
  while(true){
    if(driveTaskActive){
      float current = getDistanceInches();
      // float x = WC * gearRatio * LM.position(rev);
      float error = distanceTarget - current;
      float kp = 1.5;
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
        Brain.Screen.printAt(10, 200, "drive task ended");
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
  Brain.Screen.printAt(10, 50, "Task Initiated");
  while(true){
    Brain.Screen.printAt(10, 70, "Task Started");
    if(turnTaskActive){
      Brain.Screen.printAt(10, 90, "Robot Turning");
      float theta = imu.rotation(deg);
      float error = desiredHeading - theta;
      float kp = 0.5;
      // float speed = error * kp;
      Brain.Screen.printAt(10, 110, "Angle = %0.1f", theta);
      Brain.Screen.printAt(10, 130, "Angle = %0.1f", error);

      if(fabs(error) > 1.5){
        float correctionSpeed = kp * error;
        LM.spin(fwd, correctionSpeed, pct);
        RM.spin(fwd, -correctionSpeed, pct);
        LB.spin(fwd, correctionSpeed, pct);
        RB.spin(fwd, -correctionSpeed, pct);
      }
      else{
        stopDrive();
        Brain.Screen.printAt(10, 150, "Task Finished WOOHOO!!");
        turnTaskActive = false;
      }
    }
    wait(20, msec);
  }
  return 0;
}


void turnDistance(float degrees){
  desiredHeading = degrees;
  turnTarget = degrees;
  turnTaskActive = true;
  while(turnTaskActive){
    wait(10, msec);
  }
}


/*---------------------------------------------------------------------------*/

void pre_auton(void) {
  imu.calibrate();
  while(imu.isCalibrating()) wait(50, msec);
  myRobot = {72, 12, 90};
  Brain.Screen.printAt(10, 20, "IMU Heading: %0.1f (Should be 90)", imu.heading());
  wait(1, sec);
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

  /*
  task dc (driveTask);
  task tc (turnTask);
  wait(500, msec);

  driveDistance(24);
  turnDistance(90);
  wait(5, sec);
  turnDistance(180);

  dc.stop();
  tc.stop();
  
  Brain.Screen.printAt(10, 50, "final degrees: %0.1f", imu.rotation(deg));
  Brain.Screen.printAt(10, 70, "final distance: %0.1f", getDistanceInches());
  */
 
  // moveForward(24);
  // turnToAngle(90);

  goToPoint(72, 24);
  goToPoint(120, 24);

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
