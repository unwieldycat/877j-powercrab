using namespace vex;

extern brain Brain;

// VEXcode devices
extern controller Controller1;
extern limit LimitSwitchA;
extern drivetrain Drivetrain;
extern motor forkliftMotor1;
extern motor forkliftMotor2;
extern motor intakeMotor;
extern motor liftMotor;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );