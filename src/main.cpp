// Written by Thurston Yates for team 877J
// 2021-2022

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// Controller1          controller
// LimitSwitchA         limit         A
// Drivetrain           drivetrain    3, 2, 6, 1
// forkliftMotor1       motor         4
// forkliftMotor2       motor         7
// intakeMotor          motor         5
// liftMotor            motor         11
// ---- END VEXCODE CONFIGURED DEVICES ----

// ================================ Imports ================================ //

#include "autonutils.h"
#include "vex.h"
#include "ui.h"
#include <functional>
#include <sstream>
#include <math.h>
#include <map>

using namespace vex;

// ================================ Globals ================================ //

autonutils::RoutineManager routineManager;
competition Competition;
int selectedAutonRoutine;
bool reversed = false;
bool turbo = false;
int origin;

// ============================= Control Loops ============================= //

void liftControlLoop()
{
	bool liftActive = false;
	bool intakeActive = false;
	bool brakeLift = true;

	while (Competition.isDriverControl())
	{
		bool const R2Pressing = Controller1.ButtonR2.pressing();
		bool const R1Pressing = Controller1.ButtonR1.pressing();
		bool const UpPressing = Controller1.ButtonUp.pressing();
		bool const DownPressing = Controller1.ButtonDown.pressing();

		// Check if button B is pressing when bucket is closed and open it
		if (UpPressing && !DownPressing)
		{
			intakeActive = true;
			intakeMotor.spin(forward);
		}

		// Check if button B is pressing when bucket is open and close it
		if (DownPressing && !UpPressing)
		{
			intakeActive = true;
			intakeMotor.spin(reverse);
		}

		// Turn off intake if no input
		if (!UpPressing && !DownPressing && intakeActive)
		{
			intakeActive = false;
			intakeMotor.stop();
		}

		// Check if running without user input and stop
		if (!R2Pressing && !R1Pressing && liftActive)
		{
			liftMotor.stop(brake);
			liftActive = false;
			brakeLift = true;
		}

		if (brakeLift)
		{
			liftMotor.stop(brake);
		}

		// Listen for reverse input
		if (R2Pressing && !R1Pressing)
		{
			liftMotor.spin(reverse, 100, pct);
			liftActive = true;
			brakeLift = false;
		}

		// Listen for foward input
		if (R1Pressing && !R2Pressing)
		{
			liftMotor.spin(forward, 100, pct);
			liftActive = true;
			brakeLift = false;
		}
	}
}

void forkliftControlLoop()
{
	bool forkliftActive = false;

	while (Competition.isDriverControl())
	{
		bool const L2Pressing = Controller1.ButtonL2.pressing();
		bool const L1Pressing = Controller1.ButtonL1.pressing();

		// Check if running without user input and stop
		if ((!L2Pressing && !L1Pressing) && forkliftActive)
		{
			forkliftActive = false;
			forkliftMotor1.stop(coast);
			forkliftMotor2.stop(coast);
		}

		// Listen for reverse input
		if (L2Pressing && !L1Pressing && !LimitSwitchA.pressing())
		{
			forkliftMotor1.spin(reverse, 100, pct);
			forkliftMotor2.spin(reverse, 100, pct);
			forkliftActive = true;
		}

		// Listen for foward input
		if (L1Pressing && !L2Pressing)
		{
			forkliftMotor1.spin(forward, 100, pct);
			forkliftMotor2.spin(forward, 100, pct);
			forkliftActive = true;
		}
	}
}

void driveControlLoop()
{
	bool driving = false;
	bool turning = false;

	while (Competition.isDriverControl())
	{
		int const YPos = (reversed) ? -(Controller1.Axis3.position()) : Controller1.Axis3.position();
		int const XPos = (reversed) ? -(Controller1.Axis1.position()) : Controller1.Axis1.position();

		// Foward-backward movement
		// Check if control input is greater than 5 for deadzones
		if (YPos > 5 || YPos < -5)
		{
			Drivetrain.setDriveVelocity(abs((turbo) ? YPos : YPos / 2), pct);
			if (YPos < 0)
				Drivetrain.drive(reverse);
			if (YPos > 0)
				Drivetrain.drive(forward);
			driving = true;
		}
		else if (driving)
		{
			Drivetrain.setDriveVelocity(0, pct);
			driving = false;
		}

		// left-right movement
		// Check if control input is greater than 5 for deadzones
		if (XPos > 5 || XPos < -5)
		{
			Drivetrain.setTurnVelocity(XPos, pct);
			Drivetrain.turn(right);
			turning = true;
		}
		else if (turning)
		{
			Drivetrain.setTurnVelocity(0, pct);
			turning = false;
		}

		vex::wait(20, msec);
	}
}

void buttonListener()
{
	bool debounceY = false;
	bool debounceX = false;
	while (Competition.isDriverControl())
	{
		bool const buttonYPressing = Controller1.ButtonY.pressing();
		bool const buttonXPressing = Controller1.ButtonX.pressing();

		// Listen for button Y to be pressed
		if (buttonYPressing && !debounceY)
		{
			debounceY = true;
			reversed = !reversed;
		}
		else if (!buttonYPressing && debounceY)
			debounceY = false;

		// Listen for button X to be pressed
		if (buttonXPressing && !debounceX)
		{
			debounceX = true;
			turbo = !turbo;
		}
		else if (!buttonXPressing && debounceX)
			debounceX = false;
	}
}

// =================================== UI =================================== //

void driveUI()
{
	while (true)
	{
    // Default competition mode label
    ui::Textlabel modeLabel("Disabled", 240, 120, 0.5, 0.5);
    
    // Set background and status text depending on game mode
    if (Competition.isEnabled()) {
      if (Competition.isAutonomous()) {
        Brain.Screen.clearScreen(vex::color(255, 128, 0));
        modeLabel.setText("Autonomous");
      } 

      if (Competition.isDriverControl()) {
        Brain.Screen.clearScreen(vex::color(0, 128, 255));
        modeLabel.setText("Driver");
      }
    } else Brain.Screen.clearScreen();

    // Render mode label
    modeLabel.render();

    // Wait before next cycle to reduce cpu load
		wait(1, sec);
	}
}

void selectionUI()
{
	// Initalize variables
	bool selected;

	// Draw first step
	ui::Textlabel stepLabel = ui::Textlabel("Select field origin position", 240, 0, 0.5, 0);

	ui::Button leftButton = ui::Button(ui::Shape::Rect, 0, 240, 240, 100, 0, 1);
	leftButton.setColor(color(0, 0, 255));
	leftButton.setText("Left");
	leftButton.render();

	ui::Button rightButton = ui::Button(ui::Shape::Rect, 480, 240, 240, 100, 1, 1);
	rightButton.setColor(color(255, 0, 0));
	rightButton.setText("Right");
	rightButton.render();

	// Await user selection
	selected = false;

	while (!selected)
	{
		if (leftButton.pressing())
		{
			origin = autonutils::FieldOrigin::Left;
			selected = true;
		}

		if (rightButton.pressing())
		{
			origin = autonutils::FieldOrigin::Right;
			selected = true;
		}
	}

	// Clear screen
	Brain.Screen.clearScreen();

	// Draw next step
	stepLabel.setText("Select routine");

	// Draw routine label
	ui::Textlabel routineLabel = ui::Textlabel("X", 240, 120, 0.5, 0.5);
	routineLabel.render();

	// Get routines from routineManager
	std::vector<int> routines = routineManager.find(autonutils::FieldOrigin::Left);

	// Inline function to update routine label
	auto const updateRoutineLabel = [&](int id) -> void {
		// Convert routine id at index to string
		std::ostringstream stringified;
		stringified << id;
		
		// Re-render
		routineLabel.setText(stringified.str());
		routineLabel.render();
	};
	
	// Render buttons
	ui::Button down = ui::Button(ui::Shape::Rect, 0, 240, 100, 100, 0, 1);
	down.setColor(vex::color(255, 0, 0));
	down.setText("Down");
	down.render();

	ui::Button up = ui::Button(ui::Shape::Rect, 480, 240, 100, 100, 1, 1);
	up.setColor(vex::color(0, 255, 0));
	up.setText("Up");
	up.render();

	ui::Button done = ui::Button(ui::Shape::Rect, 240, 120, 100, 100, 0.5, 1);
	up.setColor(vex::color(0, 0, 255));
	up.setText("Done");
	up.render();

	// Await user selection
	int routineIndex = 0;
	selected = false;
	
	while (!selected) {
		if (down.pressing() && routineIndex > 0) {
			routineIndex--;
			selectedAutonRoutine = routines[routineIndex];
			updateRoutineLabel(selectedAutonRoutine);
		}

		if (up.pressing() && routineIndex < routines.size() - 1) {
			routineIndex++;
			selectedAutonRoutine = routines[routineIndex];
			updateRoutineLabel(selectedAutonRoutine);
		}

		if (done.pressing()) selected = true;
	}

	// Clear screen and reset pen
	Brain.Screen.clearScreen();
	Brain.Screen.setFillColor(transparent);
}

// ============================== Main Methods ============================== //

void pre_auton(void)
{
	vexcodeInit();
	selectionUI();
	driveUI();

	routineManager.add(0, autonutils::FieldOrigin::Both, [&]() -> void {
		liftMotor.spinFor(360, deg, true);
		Drivetrain.driveFor(10, distanceUnits::cm, true);
		intakeMotor.spin(forward); 
	});

	routineManager.add(1, autonutils::FieldOrigin::Both, [&]() -> void {
		forkliftMotor1.spinFor(forward, 360, deg, false);
		forkliftMotor2.spinFor(forward, 360, deg, false);
		Drivetrain.driveFor(75, distanceUnits::cm, true);
		Drivetrain.driveFor(-75, distanceUnits::cm, true);
		forkliftMotor1.spinFor(reverse, 360, deg, false);
		forkliftMotor2.spinFor(reverse, 360, deg, false); 
	});
}

void autonomous(void)
{
	routineManager.exec(selectedAutonRoutine);
}

void usercontrol(void)
{
	thread driveLoop = thread(driveControlLoop);
	thread forkLoop = thread(forkliftControlLoop);
	thread btnListener = thread(buttonListener);
	thread liftLoop = thread(liftControlLoop);

	btnListener.join();
	forkLoop.join();
	liftLoop.join();
	driveLoop.join();
}

// =============================== Entrypoint =============================== //

int main()
{
	// Set up callbacks for autonomous and driver control periods.
	Competition.autonomous(autonomous);
	Competition.drivercontrol(usercontrol);

	// Run the pre-autonomous function.
	pre_auton();

	// Prevent main from exiting with an infinite loop.
	while (true)
	{
		vex::wait(100, msec);
	}
}
