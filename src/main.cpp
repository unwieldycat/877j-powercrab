// Written by Thurston Yates for team 877J
// 2021-2022

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// forkliftMotor        motor         3               
// Drivetrain           drivetrain    1, 2, 20        
// Controller1          controller                    
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"

using namespace vex;

// ================= Globals ================= //

competition Competition;
int selectedAutonRoutine;

// ============ Component Actions ============ //

// TODO: Create actions for each component of the
// robot (e.g. bucket, belt, arms)

// =========== Autonomous Routines =========== //

typedef void (*autonRoutineFn) (void);

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

  // Actions
  Drivetrain.driveFor(900, mm, true);
  Drivetrain.turnToHeading(90, deg, true);
  Drivetrain.driveFor(2100, mm, true);
  Drivetrain.turnToHeading(270, deg, true);
  Drivetrain.driveFor(2100, mm, true);
  Drivetrain.turnToHeading(0, deg, true);

  /*
    TODO:
    > Drive up to goal
    > Use bucket
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
  autonRoutineFn routines[] = 
  { 
    autonRoutine0,
    autonRoutine1 
  };

  routines[selectedAutonRoutine]();
}

void usercontrol(void) {
  // Variables that need to be used between loops
  bool forkliftActive = false;

  while (1) {
    bool const L2Pressing = Controller1.ButtonL2.pressing();
    bool const L1Pressing = Controller1.ButtonL2.pressing();

    if (!L2Pressing && !L1Pressing && forkliftActive) {
      forkliftMotor.stop(coast);
      forkliftActive = false;
    } 

    if (L2Pressing && !L1Pressing) {
      forkliftMotor.spin(reverse, 100, pct);
      forkliftActive = true;
    }

    if (L1Pressing && !L2Pressing) {
      forkliftMotor.spin(forward, 100, pct);
      forkliftActive = true;
    }

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
