/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       team7700                                                  */
/*    Created:      4/3/2025, 6:33:44 PM                                      */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "vex.h"

using namespace vex;

// A global instance of competition
competition Competition;
motor RFM(PORT13, ratio6_1, false);
motor RBM(PORT12, ratio6_1, false);
motor LFM(PORT9, ratio6_1, true);
motor LBM(PORT15, ratio6_1, true);
inertial imu(PORT2);
brain Brain;

// define your global instances of motors and other devices here
float dia = 2.75;
float gearRatio = 1;
float WC = dia * M_PI;

float distanceTarget = 0;
bool driveTaskActive = false;


/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
 
float getDistanceInches() {
  float LFrevs = LFM.position(rev) * gearRatio;
  float RFrevs = RFM.position(rev) * gearRatio;
  float avgRevs = (LFrevs + RFrevs)/2 ;
  return avgRevs * WC;
}

void resetDrive() {
  LFM.resetPosition();
  RFM.resetPosition();
  imu.resetRotation();
  driveTaskActive = false;

}

int driveTask() {
  while (true) {
    if (driveTaskActive){

  float current = getDistanceInches();
  float error = distanceTarget - current;
  float kp = 2.5;
  float speed = error * kp;

  LFM.spin(forward, speed, pct);
  LBM.spin(forward, speed, pct);
  RFM.spin(forward, speed, pct);
  RBM.spin(forward, speed, pct);

  if (fabs (error) < 0.2) {
    LFM.stop(brake);
    LBM.stop(brake);
    RFM.stop(brake);
    RBM.stop(brake);
    driveTaskActive = false;
  }
}
  wait(20, msec);
}
return 0;
}

void driveDistance(float inches) {
  resetDrive();
  distanceTarget = inches;
  driveTaskActive = true;
  while (driveTaskActive){
    wait(10,msec);
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
vex::thread dc (driveTask); 
wait (500, msec); 
driveDistance(24); 
dc.interrupt();  

Brain.Screen.printAt(10, 20, "final distance: %0.1f", getDistanceInches()); 


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
