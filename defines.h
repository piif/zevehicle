// PROGRAM SETUP
// **************

#define DELAY_BETWEEN_INSTRUCTIONS 200
#define PROGRAM_MAX 20

// STEPPERS SETUP
// **************

#define ENABLE_STEPPERS 3

// pinout for stepper motors
#define STEPPER_LEFT_A   4
#define STEPPER_LEFT_B   5  
#define STEPPER_LEFT_C   6
#define STEPPER_LEFT_D   7

#define STEPPER_RIGHT_A  8
#define STEPPER_RIGHT_B  9
#define STEPPER_RIGHT_C 10
#define STEPPER_RIGHT_D 11

// on lego prototype :
// 51 steps to move forward/backward 5cm
// 76 steps to turn a quarter
// speed 20 a bit slow, but with speed 10 first steps are weird
// -> TODO start  at 20 and continue at 10 after some steps, then slow back to 20 before last steps ?

#define TURN_STEPS 86    // for a quarter turn
#define FORWARD_STEPS 51 // for 5cm

// delay between steps (ms) at startup and before last steps (NOT YET IMPLEMENTED)
#define LOW_SPEED 20
#define LOW_SPEED_STEPS 5
// delay between steps (ms)
#define HIGH_SPEED  10

// left motor is 96 steps, right one is 48 steps
// -> by setting rigth motor on half steps, wheels are 96 steps per rotation
// todo : try to use micro steps instead (4th and 8th steps) to have smooth movement ?

#define LEFT_MODE_HALF  0
#define RIGHT_MODE_HALF 1

// REMOTE_SETUP
// **************

// TODO : pass these values as setup parameters
#define REMOTE_INPUT 2
#define REMOTE_INTR 0

// SCREEN SETUP
// **************

#define SCREEN_SIZE  4
#define SCREEN_CLK  A5
#define SCREEN_DS   A4
#define SCREEN_DIN  A3 // 12