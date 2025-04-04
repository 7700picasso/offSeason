/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       Team7700                                                  */
/*    Created:      4/3/2025, 6:32:52 PM                                      */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "vex.h"

using namespace vex;

// A global instance of competition
competition Competition;
brain Brain;
motor LM(PORT11, ratio6_1, true);
motor RM(PORT10, ratio6_1, false);
motor Lb(PORT2, ratio6_1, true);
motor Rb(PORT19, ratio6_1, false);
inertial imu(PORT13);


// define your global instances of motors and other devices here
float dia = 4;
float gearRatio = 0.4;
float WC = dia * M_PI;
float distanceTarget = 0;
bool drivetaskactive = false;
/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */

float getdistanceinches() {
  float LMrevs = LM.position(rev) * gearRatio;
  float Rmrevs = RM.position(rev) * gearRatio;
  float avgRevs = (LMrevs + Rmrevs)/2 ;
  return avgRevs * WC;
}

void resetDrive() {
  LM.resetPosition();
  RM.resetPosition();
  imu.resetRotation();
  drivetaskactive = false;
}

int drivetask() {
  
  while (true) {
    if (drivetaskactive) { 
  float current = getdistanceinches();
  float error = distanceTarget - current;
  float kp = 1.5;
  float speed = error * kp;
  RM.spin(forward, speed, pct);
  LM.spin(forward, speed, pct);
  Rb.spin(forward, speed, pct);
  Lb.spin(forward, speed, pct);

  if (fabs (error) < 0.5){
    LM.stop(brake);
    RM.stop(brake);
    Rb.stop(brake);
    Lb.stop(brake);
    drivetaskactive = false;
  }
  }
wait(20, msec);
}
  return 0;
}

void drivedistance(float inches){
  resetDrive();
  distanceTarget = inches;
  drivetaskactive = true;
  while (drivetaskactive){
    wait(10, msec);
  }
}

/*---------------------------------------------------------------------------*/

void pre_auton(void) {
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
  task dc (drivetask);
  wait(500, msec);
  drivedistance(24);

  dc.stop();
  Brain.Screen.printAt(10, 50, "fina Dist: %0.1f", getdistanceinches());


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
