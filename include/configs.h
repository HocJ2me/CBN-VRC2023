
// #define MAX_PWM 800
#define MIN_PWM 50
#define BASE_LINE_PWM 500

//mid joystick has value: 128 -> offset it to safe in control
#define NOISE_J_UP      140
#define NOISE_J_DOWN    116

#define MAX_SPEED 4000
#define SHOOT_MOTOR_SPEED (4000*3/2)
#define COLECT_MOTOR_SPPED (MAX_SPEED)
#define GET_BALL 1
#define ROTATE_SPEED_SCALE 0.75

uint16_t MAX_PWM = 1400; // tốc độ motor

#define LEFT_MOTOR      1
#define RIGHT_MOTOR     2
#define COLLECT_MOTOR      3
#define SHOOT_MOTOR    4

#define PICK_UP_SERVO1   3
#define PICK_UP_SERVO2   4

#define HOLDER_SERVO    1
#define HOLDER_SERVO2   2

#define RONTARY_SERVO    3
#define RONTARY_SERVO2   4

#define OSICILLATOR_FREQUENCY 27000000
#define PWM_FREQUENCY 60
#define I2C_COM_CLOCK 400000
#define MAX_PWM_VAL 4095


#define PS2_DAT 39
#define PS2_CMD 13
#define PS2_SEL 15
#define PS2_CLK 14
#define pressures false
#define rumble false