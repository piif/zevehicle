// PROGRAM SETUP
// **************

#define DELAY_BETWEEN_INSTRUCTIONS 200
#define PROGRAM_MAX 20

// STEPPERS SETUP
// **************

#define ENABLE_STEPPERS A5

// pinout for stepper motors
#define STEPPER_LEFT_A   13
#define STEPPER_LEFT_B   12  
#define STEPPER_LEFT_C   11
#define STEPPER_LEFT_D    3

#define STEPPER_RIGHT_A  A4
#define STEPPER_RIGHT_B  A3
#define STEPPER_RIGHT_C  A2
#define STEPPER_RIGHT_D  A1

// on lego prototype :
// 51 steps to move forward/backward 5cm
// 76 steps to turn a quarter
// speed 20 a bit slow, but with speed 10 first steps are weird
// -> TODO start  at 20 and continue at 10 after some steps, then slow back to 20 before last steps ?

#define TURN_STEPS 86    // for a quarter turn
#define FORWARD_STEPS 51 // for 5cm

// delay between steps (ms) at startup and before last steps (NOT YET IMPLEMENTED)
#define LOW_SPEED 30
#define LOW_SPEED_STEPS 5
// delay between steps (ms)
#define HIGH_SPEED  20

// left motor is 48 steps, right one is 96 steps
// -> by setting left motor on half steps, wheels are 96 steps per rotation
// todo : try to use micro steps instead (4th and 8th steps) to have smooth movement ?

#define LEFT_MODE_HALF  1
#define RIGHT_MODE_HALF 0

// REMOTE_SETUP
// **************

// TODO : pass these values as setup parameters
#define REMOTE_INPUT 2
#define REMOTE_INTR 0

// LCD SCREEN SETUP
// **************

#define SCREEN_ENABLE    9
#define SCREEN_READ_SEL  8
#define SCREEN_D4        4
#define SCREEN_D5        5
#define SCREEN_D6        6
#define SCREEN_D7        7
#define SCREEN_LIGHT    10
#define SCREEN_BUTTONS  A0
