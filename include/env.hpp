#ifndef ENV_HPP
# define ENV_HPP

#define SENSOR_PINS     (int []){ A1, A2, A3, A4, A5, -1 }
#define MOTOR_A         (int []){ 11, 5, 4 } // ENA, IN1, IN2
#define MOTOR_B         (int []){ 3, A0, 2 } // ENB, IN3, IN4
#define GRIPPER_PIN     6
#define GRIPPER_ARM_PIN 9

// Storage selection
#define SWITCH_BLK1        12
#define SWITCH_BLK3        7

// BLK2 = BLK1 && BLK3

// Direction and Debug
#define SWITCH_L           8

// Forward bias = L && R

// Speed
#define MAX_SPEED       165
#define MID_SPEED       150
#define MIN_SPEED       130

// Sensor threshold
#define THRESH          200
#define LINE_THRESH     1023 * 1.3f

// PID Controller

#define KP              0.8600f
#define KI              0.5400f
#define KD              0.0005f
#define HZ              500
#define DELAY           1000 / HZ

// Interpreter max line count
#define MAX_PROG        50

#endif
