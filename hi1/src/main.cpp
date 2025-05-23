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
controller Controller;
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

float desireheading = 0;
float angletarget = 0;
bool turntaskactive = 0;
/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */

struct robot{
  float x =0;
  float y =0;
  float heading = 0;

      robot(double x_init = 0, double y_init = 0, double heading_init = 0):
       x(x_init), y(y_init), heading(heading_init) {}

}myRobot;

void moveforward(float targetinches){
  float distance = 0;
  float error = targetinches - distance;
  float kp = 3.0;
  float speed = error * kp;
  Lb.resetPosition();
  Rb.resetPosition();

  while (fabs (error) > 0.5){
    distance = (LM.position(deg) +RM.position(deg)) /2.0*WC * gearRatio;
    error = targetinches - distance;
    speed = error * kp;
    LM.spin(forward, speed, pct);
    RM.spin(forward, speed, pct);
    Lb.spin(forward, speed, pct);
    Rb.spin(forward, speed, pct);
  }
  LM.stop();
  LM.stop();
  LM.stop();
  LM.stop();

  myRobot.x += targetinches * cos(myRobot.heading * (3.14/180));
  myRobot.y += targetinches * sin(myRobot.heading * (3.13/180));

}

void turnToAngle(double targetDegrees){
  double kp = 0.8;
  double tolerance = 2.0;

  while(true){
    // double error = targetdegrees - imu.heading();
    double currentHeading = imu.heading();
      if (currentHeading > 180) currentHeading -= 360;
      Brain.Screen.printAt(10, 100, "Normalized Heading: %.1f", currentHeading);

      // keep error between -180 and 180
      double error = targetDegrees - currentHeading;
      if (error > 180) error -= 360;
      if (error < -180) error += 360;
      Brain.Screen.printAt(10,115,"Current; %.1f, Target: %.1f, Error: %.1f",
                      currentHeading, targetDegrees, error);
      if (fabs(error)  < tolerance) break;

      double speed = error *kp;
      LM.spin(forward,speed,pct);
      Lb.spin(forward,-speed,pct);
      RM.spin(forward,speed,pct);
      Rb.spin(forward,-speed,pct);
      wait(20,msec);
    
  }
  LM.stop();
  Lb.stop();
  RM.stop();
  Rb.stop();
  myRobot.heading = targetDegrees;
}

void gotopoint(double targetX, double targetY){
  double dx = targetX - myRobot.x;
  double dy = targetY - myRobot.y;  

  double targetAngle = atan2(dy, dx) * (180.0/3.14);
  Brain.Screen.printAt(10, 160, "heading=", targetAngle);

  if (fabs(targetAngle - myRobot.heading) > 5.0){
    turnToAngle(targetAngle);
  }
  double distance = sqrt(dx*dx + dy*dy);
  moveforward(distance);

  Brain.Screen.printAt(10,80,"Reached (%.1f, %.1f)!", targetX, targetY);
  Controller.rumble(".");
}




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
  turntaskactive = false;
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


int turntask () {
  Brain.Screen.printAt(10,50, "task initialized");
  while(true) {
     Brain.Screen.printAt(10,70, "task started");
    if (turntaskactive){
       Brain.Screen.printAt(10,50, "robot turning");
      float currentangle = imu.rotation();
      float error = desireheading - currentangle;
      Brain.Screen.printAt(10,100, "Current heading= %0.1f", currentangle);
      Brain.Screen.printAt(10,120, "Current error= %0.1f", error);
      if (fabs (error)>1.5){
        float correction = error * 0.5;
        LM.spin(forward, correction, pct);
        Lb.spin(forward, correction, pct);
        RM.spin(forward, -correction, pct);
        Rb.spin(forward, -correction, pct);
      }
      else{
        LM.stop(brake);
        RM.stop(brake);
        Lb.stop(brake);
        Rb.stop(brake);
        turntaskactive = false;
      }
    }
    wait(20,msec);
  }
  return 0;
}
void turntoangle(float degrees){
  desireheading = degrees;
  angletarget = degrees;
  turntaskactive = true;
  while (turntaskactive){
    wait(10,msec);
  }
}



/*---------------------------------------------------------------------------*/

void pre_auton(void) {
resetDrive();
  while(imu.isCalibrating()) wait(50,msec);
  myRobot = {72, 12, 90};
  Brain.Screen.printAt(10, 20, "IMU Heading; %.1f Should be 90");
  wait(1, msec);
  
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
  task dc (drivetask);
  task tc (turntask);
  wait(500, msec);
  drivedistance(24);
  turntoangle(90);
  wait(5,sec);
  turntoangle(180);
  drivedistance(24);

  tc.stop();
  dc.stop();
  Brain.Screen.printAt(10, 50, "fina Dist: %0.1f", getdistanceinches());
  Brain.Screen.printAt(10, 90, "final angle: %0.1f", imu.rotation()); 
  */

  // moveforward(24);
  //turntoangle(90);

  gotopoint(72, 24);
  gotopoint(120,24);

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
