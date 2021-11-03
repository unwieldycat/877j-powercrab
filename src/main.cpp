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
#include <sstream>

using namespace vex;

// ================= Globals ================= //

competition Competition;
int selectedAutonRoutine;

// ============ Component Actions ============ //

// TODO: Create actions for each component of the
// robot (e.g. bucket, belt, arms)

// =========== Autonomous Routines =========== //

// Type definition for routine function
typedef void (*autonRoutineFn) (void);

// Routine 0
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

// Routine 1
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
  Drivetrain.driveFor(reverse, 450, mm, true);

  /*
    TODO:
    > Use bucket
  */
}

// TODO: Create more routines

// Register all autonomous routines into a global for accessibility within
// various functions
autonRoutineFn routines[] = 
{ 
  autonRoutine0,
  autonRoutine1 
};

// ========= Main Competition Methods ========= //

void pre_auton(void) {
  vexcodeInit();

  // TODO: Initialize robot components
  // (e.g. retract arms, lower buckets, calibrate sensors)

  bool iterationDebounce = false;
  bool selected = false;
  while (!selected) {

    // Read user input
    bool const RightPressing = Controller1.ButtonRight.pressing();
    bool const LeftPressing = Controller1.ButtonLeft.pressing();
    bool const ButtonAPressing = Controller1.ButtonA.pressing();

    if (
      LeftPressing &&
      selectedAutonRoutine > 0 && 
      !iterationDebounce
    ) {
      iterationDebounce = true;
      selectedAutonRoutine--;
    }

    if (
      RightPressing &&
      !iterationDebounce &&
      selectedAutonRoutine < 10
    ) {
      iterationDebounce = true;
      selectedAutonRoutine++;
    }
    
    if (!RightPressing && !LeftPressing) iterationDebounce = false;

    if (ButtonAPressing) selected = true;

    // Convert selection to string
    std::ostringstream stringified;
    stringified << "Selected routine: " << selectedAutonRoutine;
    std::string const message = stringified.str();

    // Draw selection
    Brain.Screen.clearScreen();
    Brain.Screen.setCursor(1, 1);
    Brain.Screen.print(message.c_str());

    // Wait before exiting loop to avoid wasted resources
    wait(20, msec);
  }

  // Print confirmation message
  Brain.Screen.clearScreen();
  Brain.Screen.setCursor(1, 1);
  Brain.Screen.print("Selected routine, waiting for autonomous period");
  Brain.Screen.setCursor(2, 1);
  Brain.Screen.print("to begin...");
}

void autonomous(void) {
  // Epic one liner which runs selected autonomous routine
  routines[selectedAutonRoutine]();
}

void usercontrol(void) {
  // Variables that need to be used between loops
  bool forkliftActive = false;
  bool reversed = false;
  bool driving = false;
  bool turning = false;
  bool turbo = false;

  bool buttonYDebounce = false;

  while (true) {
    // Reversed driving mode
    bool const buttonYPressing = Controller1.ButtonY.pressing();

    if (buttonYPressing && !buttonYDebounce) {
      buttonYDebounce = true;
      reversed = !reversed;
    }
    
    if (!buttonYPressing && buttonYDebounce) buttonYDebounce = false;

    // Drive control section
    int const YPos = (reversed) ? -(Controller1.Axis3.position()) : Controller1.Axis3.position();
    int const XPos = (reversed) ? -(Controller1.Axis1.position()) : Controller1.Axis1.position(); 

    // Foward-backward movement
    // Check if control input is greater than 5 for deadzones
    if (YPos > 5 || YPos < -5) {
      Drivetrain.setDriveVelocity(abs((turbo) ? YPos : YPos / 2), pct);
      if (YPos < 0) Drivetrain.drive(reverse);
      if (YPos > 0) Drivetrain.drive(forward);
      driving = true;
    } else if (driving) {
      Drivetrain.setDriveVelocity(0, pct);
    }

    // left-right movement
    // Check if control input is greater than 5 for deadzones
    if (XPos > 5 || XPos < -5) {
      Drivetrain.setTurnVelocity(abs(YPos), pct);
      if (YPos < 0) Drivetrain.turn(left);
      if (YPos > 0) Drivetrain.turn(right);
      turning = true;
    } else if (turning) {
      Drivetrain.setTurnVelocity(0, pct);
    }

    // Forklift control
    bool const L2Pressing = Controller1.ButtonL2.pressing();
    bool const L1Pressing = Controller1.ButtonL2.pressing();

    // Check if running without user input and stop
    if (!L2Pressing && !L1Pressing && forkliftActive) {
      forkliftMotor.stop(coast);
      forkliftActive = false;
    } 

    // Listen for reverse input
    if (L2Pressing && !L1Pressing) {
      forkliftMotor.spin(reverse, 100, pct);
      forkliftActive = true;
    }

    // Listen for foward input
    if (L1Pressing && !L2Pressing) {
      forkliftMotor.spin(forward, 100, pct);
      forkliftActive = true;
    }

    // Wait before exiting loop to avoid wasted resources
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
