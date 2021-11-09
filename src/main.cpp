// Written by Thurston Yates for team 877J
// 2021-2022

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// forkliftMotor        motor         3               
// Drivetrain           drivetrain    1, 2, 20        
// Controller1          controller                    
// liftMotor            motor         4               
// intakeMotor          motor         5               
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"
#include <sstream>

using namespace vex;

// ================= Globals ================= //

competition Competition;
int selectedAutonRoutine;
bool reversed = false;
bool turbo = false;

// =========== Autonomous Routines =========== //

// Type definition for routine function
typedef void (*autonRoutineFn) (void);

// Routine 0
void autonRoutine0() {
  // Initialize
  Drivetrain.setHeading(0, deg);
  Drivetrain.setDriveVelocity(100, pct);

  // Actions
  Drivetrain.driveFor(950, mm, true);
  Drivetrain.turnToHeading(90, deg, true);
  Drivetrain.driveFor(2250, mm, true);
  Drivetrain.turnToHeading(315, deg, true);
  Drivetrain.setDriveVelocity(25, pct);
  Drivetrain.driveFor(-150, mm, true);

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

  // Collect rings and drive
  intakeMotor.spin(forward);
  Drivetrain.driveFor(950, mm, true);
  Drivetrain.turnToHeading(90, deg, true);
  Drivetrain.driveFor(2100, mm, true);
  Drivetrain.turnToHeading(270, deg, true);
  Drivetrain.driveFor(2050, mm, true);
  Drivetrain.turnToHeading(0, deg, true);
  Drivetrain.driveFor(reverse, 450, mm, true);

  // Drop rings in bucket
  liftMotor.setPosition(0, deg);
  liftMotor.spinToPosition(180, deg);
  intakeMotor.spin(reverse);
  wait(1, sec);
  intakeMotor.stop(coast);
  liftMotor.spinToPosition(0, deg, true);
}

// TODO: Create more routines

// Register all autonomous routines into a global for accessibility within
// various functions
autonRoutineFn routines[] = 
{ 
  autonRoutine0,
  autonRoutine1 
};

// ============== Control Loops ============== //

void liftControlLoop() {
  bool liftActive = false;
  bool intakeActive = false;

  liftMotor.resetPosition();
  while(Competition.isDriverControl()) {
    bool const R2Pressing = Controller1.ButtonR2.pressing();
    bool const R1Pressing = Controller1.ButtonR1.pressing();
    bool const UpPressing = Controller1.ButtonUp.pressing();
    bool const DownPressing = Controller1.ButtonDown.pressing();

    // Check if button B is pressing when bucket is closed and open it
    if (UpPressing && !DownPressing) {
      intakeActive = true;
      intakeMotor.spin(forward);
    }

    // Check if button B is pressing when bucket is open and close it
    if (DownPressing && !UpPressing) {
      intakeActive = true;
      intakeMotor.spin(reverse);
    }

    // Turn off intake if no input
    if (!UpPressing && !DownPressing && intakeActive) {
      intakeActive = false;
      intakeMotor.stop();
    } 

    // Check if running without user input and stop
    if (!R2Pressing && !R1Pressing && liftActive) {
      liftMotor.stop(coast);
      liftActive = false;
    } 

    // Listen for reverse input
    if (R2Pressing && !R1Pressing) {
      liftMotor.spin(reverse, 100, pct);
      liftActive = true;
    }

    // Listen for foward input
    if (R1Pressing && !R2Pressing) {
      liftMotor.spin(forward, 100, pct);
      liftActive = true;
    }
  }
}

void forkliftControlLoop() {
  bool forkliftActive = false;

  while(Competition.isDriverControl()) {
    bool const L2Pressing = Controller1.ButtonL2.pressing();
    bool const L1Pressing = Controller1.ButtonL1.pressing();

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
  }
}

void driveControlLoop() {
  bool driving = false;
  bool turning = false;

  while(Competition.isDriverControl()) {
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
      driving = false;
    }

    // left-right movement
    // Check if control input is greater than 5 for deadzones
    if (XPos > 5 || XPos < -5) {
      Drivetrain.setTurnVelocity(XPos, pct);
      Drivetrain.turn(right);
      turning = true;
    } else if (turning) {
      Drivetrain.setTurnVelocity(0, pct);
      turning = false;
    }

    vex::wait(20, msec);
  }
}

void buttonListener() {
  bool debounceY = false;
  bool debounceX = false;
  while(Competition.isDriverControl()) {
    bool const buttonYPressing = Controller1.ButtonY.pressing();
    bool const buttonXPressing = Controller1.ButtonX.pressing();

    // Listen for button Y to be pressed
    if (buttonYPressing && !debounceY) {
      debounceY = true;
      reversed = !reversed;
    } else if (!buttonYPressing && debounceY) debounceY = false;

    // Listen for button X to be pressed
    if (buttonXPressing && !debounceX) {
      debounceX = true;
      turbo = !turbo;
    } else if (!buttonXPressing && debounceX) debounceX = false;
  }
}

void driveUI() {
  while(true) {
    // Game mode
    bool isAuton = Competition.isAutonomous();
    bool isDrive = Competition.isDriverControl();
    Brain.Screen.clearLine(1);
    
    if (isAuton) {
      std::ostringstream routineStr;
      routineStr << "(ROUTINE " << selectedAutonRoutine << ")";
      Brain.Screen.setCursor(1, 1);
      Brain.Screen.setFillColor(orange);
      Brain.Screen.print(("AUTON MODE" + routineStr.str()).c_str());
      Brain.Screen.setFillColor(transparent);
    }

    if (isDrive) {
      Brain.Screen.setCursor(1, 1);
      Brain.Screen.setFillColor(blue);
      Brain.Screen.print("DRIVE MODE");
      Brain.Screen.setFillColor(transparent);
    }

    // Turbo status
    Brain.Screen.clearLine(2);
    Brain.Screen.setCursor(2, 1);

    (turbo)
    ? Brain.Screen.print("Turbo: On")
    : Brain.Screen.print("Turbo: Off");

    // Reverse status
    Brain.Screen.clearLine(3);
    Brain.Screen.setCursor(3, 1);

    (reversed)
    ? Brain.Screen.print("Reverse: On")
    : Brain.Screen.print("Reverse: Off");

    // Heading
    double const compassHeading = Drivetrain.heading(deg);
    std::ostringstream headingStr;
    headingStr << "Heading: " << round(compassHeading);

    Brain.Screen.clearLine(4);
    Brain.Screen.setCursor(4, 1);
    Brain.Screen.print(headingStr.str().c_str());

    wait(1, sec);
  }
}

// ========= Main Competition Methods ========= //

void pre_auton(void) {
  vexcodeInit();

  // TODO: Initialize robot components
  // (e.g. retract arms, lower buckets, calibrate sensors)

  bool iterationDebounce = false;
  bool selected = false;

  Brain.Screen.setFillColor(red);
  Brain.Screen.drawRectangle(0, 190, 215, 50);
  Brain.Screen.printAt(10, 215, "Sub");
  Brain.Screen.setFillColor(green);
  Brain.Screen.drawRectangle(215, 190, 430, 50);
  Brain.Screen.printAt(225, 215, "Add");
  Brain.Screen.setFillColor(blue);
  Brain.Screen.drawRectangle(430, 190, 50, 50);
  Brain.Screen.printAt(440, 215, "Go");
  Brain.Screen.setFillColor(transparent);

  while (!selected && !Competition.isEnabled()) {
    int const xPos = Brain.Screen.xPosition();
    int const yPos = Brain.Screen.yPosition();
    bool const screenPressing = Brain.Screen.pressing();

    if (screenPressing) {
      if (
        xPos > 0 && 
        xPos < 215 &&
        yPos > 190 &&
        yPos < 240 &&
        !iterationDebounce &&
        selectedAutonRoutine > 0
      ) {
        selectedAutonRoutine--;
        iterationDebounce = true;
      }

      if (
        xPos > 215 && 
        xPos < 430 &&
        yPos > 190 &&
        yPos < 240 &&
        !iterationDebounce &&
        selectedAutonRoutine < sizeof(routines)/sizeof(routines[0]) - 1
      ) {
        selectedAutonRoutine++;
        iterationDebounce = true;
      }

      if (
        xPos > 430 && 
        xPos < 480 &&
        yPos > 190 &&
        yPos < 240
      ) {
        selected = true;
      }

    } else {
      iterationDebounce = false;
    }

    // Convert selection to string
    std::ostringstream stringified;
    stringified << "Selected routine: " << selectedAutonRoutine;
    std::string const message = stringified.str();

    // Draw selection
    Brain.Screen.clearLine(1);
    Brain.Screen.setCursor(1, 1);
    Brain.Screen.print(message.c_str());

    // Wait before exiting loop to avoid wasted resources
    wait(20, msec);
  }

  // Print confirmation message if selected
  Brain.Screen.clearScreen();
  driveUI();
}

void autonomous(void) {
  routines[selectedAutonRoutine]();
}

void usercontrol(void) {
  thread driveLoop = thread(driveControlLoop);
  thread forkLoop = thread(forkliftControlLoop);
  thread btnListener = thread(buttonListener);

  btnListener.join();
  forkLoop.join();
  driveLoop.join();
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
