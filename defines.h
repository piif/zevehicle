// PROGRAM SETUP
// **************

#define DELAY_BETWEEN_INSTRUCTIONS 200
#define PROGRAM_MAX 20

// STEPPERS SETUP
// **************

#define ENABLE_STEPPERS A5

// pinout for stepper motors
#define STEPPER_LEFT_A   12
#define STEPPER_LEFT_B    3  
#define STEPPER_LEFT_C   11
#define STEPPER_LEFT_D   13

#define STEPPER_RIGHT_A  A4
#define STEPPER_RIGHT_B  A3
#define STEPPER_RIGHT_C  A1
#define STEPPER_RIGHT_D  A2

// delay between steps (ms) at startup and before last steps (NOT YET IMPLEMENTED)
#define LOW_SPEED 30
#define LOW_SPEED_STEPS 12
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

#define SCREEN_COLS     16
#define SCREEN_ROWS      2

#define SCREEN_ENABLE    9
#define SCREEN_READ_SEL  8
#define SCREEN_D4        4
#define SCREEN_D5        5
#define SCREEN_D6        6
#define SCREEN_D7        7
#define SCREEN_LIGHT    10
#define SCREEN_BUTTONS  A0
