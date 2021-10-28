// Written by Thurston Yates for team 877J
// 2021-2022

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// forkliftMotor        motor         3               
// Drivetrain           drivetrain    1, 2, 20        
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"

using namespace vex;

// ================= Globals ================= //

competition Competition;
int autonRoutine;

// ============ Component Actions ============ //

// TODO: Create actions for each component of the
// robot (e.g. bucket, belt, arms)

// =========== Autonomous Routines =========== //

/*
  Routine 0: Sweep

  Routine 0 starts by collecting all the rings
  present on the current side of the field, and 
  dumps it into the goal furthest from the 
  starting point. This routine requires the robot 
  to start on the left of the field, halfway in 
  between the balance and the side wall.
*/
void autonRoutine0() {
  // Initialize
  Drivetrain.setHeading(0, deg);
  Drivetrain.setDriveVelocity(100, pct);
  Drivetrain.setTurnVelocity(50, pct);

  // Actions
  Drivetrain.driveFor(900, mm, true);
  Drivetrain.turnToHeading(90, deg, true);
  Drivetrain.driveFor(2150, mm, true);
  Drivetrain.turnToHeading(315, deg, true);

  /*
    TODO:
    > Move to bucket
    > Use bucket
    > Move back from goal
    > Turn 180 degrees
    > Pick up goal
  */
}

/* 
  Routine 1: Fetch

  Routine 1 collects all the rings on the current
  side of the field and places it on the bucket
  next to the starting position of the robot
*/
void autonRoutine1() {
  // Initialize
  Drivetrain.setHeading(0, deg);
  Drivetrain.setDriveVelocity(100, pct);
  Drivetrain.setTurnVelocity(50, pct);

  /*
    TODO:
    > Drive foward to be on same axis as ring clusters
    > Turn 90 degrees to the right
    > Collect both rings
    > Turn 180 degrees to come back to starting position
    > Just before starting position, face rear towards goal
    > Drive to goal and activate bucket
  */
}

// TODO: Create more routines

// ========= Main Competition Methods ========= //

void pre_auton(void) {
  vexcodeInit();

  // TODO: Initialize robot components
  // (e.g. retract arms, lower buckets, calibrate sensors)

  // TODO: Create a routine selection system
}

void autonomous(void) {
  // TODO: Run selected routine
}

void usercontrol(void) {
  while (1) {
    // TODO: Listen for changes on controller

    wait(20, msec);
  }
}

// ================ Entrypoint ================ //

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
