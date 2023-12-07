#include "line_follower.hpp"

SensorArray           sensors(SENSOR_PINS, THRESH);
FastPID               controller(KP, KI, KD, HZ);
DriverL298N           driver(MOTOR_A, MOTOR_B, MIN_SPEED, MAX_SPEED);
Gripper               gripper(GRIPPER_PIN, GRIPPER_ARM_PIN);
Robot                 robot(&sensors, &controller, &driver, &gripper);
extern unsigned long  time = 0;
extern int            delay_us = 0;

/* The map:
 [1]     [2] 4   [3] 5  6
 4|_______|_______|___|__
  |_     _5 |6   7|_  |8 |
    |   | | |3    _/    [E]
   _|   | |_|     \
  |_____|_________|_____[S]
  |     |         |
  3     2         1
*/

const enum e_cmd prog1[MAX_PROG][2] PROGMEM = {
  { _begin },
  // Go straight until end of line
  
  { _fast, 2 },
  { _mid, 1 },
  { _right },

  { _mid, 1 },

  // { _grab },

  { _abs_right },
  { _right },

  { _fast, 3 },
  { _mid, 1 },
  
  { _end }
};

// const enum e_cmd prog1[MAX_PROG][2] PROGMEM = {
//   { _begin },
//   // Go straight until end of line
//   // { _fast, 1 },
//   { _abs_right },
  
//   { _end }
// };


const enum e_cmd prog2[MAX_PROG][2] PROGMEM = {
  { _begin },
  // Go straight until end of line
  
  { _fast, 1 },
  { _slow, 1 },
  { _right },

  { _mid, 1 },
  { _left },

  { _mid, 1 },
  { _right },

  // { _grab },
  
  { _right },
  { _fast, 2 },
  { _mid, 1 },
  
  { _end }
};

const enum e_cmd prog3[MAX_PROG][2] PROGMEM = {
  { _begin },
  // Go straight until end of line
  
  { _fast, 1 },
  { _slow, 1 },
  { _right },

  { _mid, 1 },
  { _right },
  { _mid, 1 },
  { _left },

  // { _grab },

  { _right },
  { _fast, 1 },

  { _end }
};

const enum e_cmd prog3_alt[MAX_PROG][2] PROGMEM = {
  { _begin },
  // Go straight until end of line
  
  { _fast, 1 },
  { _right },

  { _mid, 1 },

  // { _grab },

  { _right },
  { _fast, 1 },

  { _end }
};


const enum e_cmd pain_reliever[MAX_PROG][2] PROGMEM = {
  { _begin },
  // Go straight until end of line

  { _fast, 1 },

  { _right },

  { _mid, 1 },

  { _right },

  { _mid, 1 },

  { _end }
};

enum e_cmd program[MAX_PROG][2];

/*
pig can fly ? what else ?? load the program then
*/
void  load_program(char *name, enum e_cmd prog[MAX_PROG][2], bool inverted)
{
  Serial.print(name);
  if (inverted)
    Serial.print(" (inverted map)");
  Serial.println(": Loading PROGMEM to SRAM...");
  program[0][0] = _end;
  for (int i = 0; i < MAX_PROG; i++)
  {
    memcpy_PF(program[i], prog[i], sizeof(e_cmd) * 2);
    if (program[i][0] == _left && inverted) program[i][0] = _right;
    else if (program[i][0] == _right && inverted) program[i][0] = _left;
    if (program[i][0] == _abs_left && inverted) program[i][0] = _abs_right;
    else if (program[i][0] == _abs_right && inverted) program[i][0] = _abs_left;
    if (program[i][0] == _end)
      break ;
    if (i + 1 == MAX_PROG)
    {
      Serial.print(name);
      Serial.println(": Invalid Program! No termination.");
      for (;;) delay(1000);
    }
  }
  Serial.println("Finished loading.");
}

/*
Ard
*/
void setup()
{
	Serial.begin(9600);
  controller.setOutputRange(-1024, 1023);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(SWITCH_BLK1, INPUT_PULLUP);
  pinMode(SWITCH_BLK3, INPUT_PULLUP);
  pinMode(SWITCH_L, INPUT_PULLUP);
  digitalWrite(LED_BUILTIN, LOW);

  bool mode1 = digitalRead(SWITCH_BLK1);
  bool mode2 = digitalRead(SWITCH_BLK3);
  bool inv = !digitalRead(SWITCH_L);
  // bool inv = !(0);
  char buffer[99];
  sprintf(buffer, "invert value: %d, trackL %d %d", digitalRead(SWITCH_L), digitalRead(SWITCH_BLK1), digitalRead(SWITCH_BLK3));
  Serial.println(buffer);

  // switch ((digitalRead(SWITCH_BLK3) << 1) + digitalRead(SWITCH_BLK1))
  // {
  //   case 1: load_program("Program storage 1", prog1, inv); break;
  //   case 2: load_program("Program storage 3", prog3, inv); break;
  //   case 3: load_program("Program storage 2", prog2, inv); break;
  // }
  // gripper.set_zero();

  if (mode1 == 1 && mode2 == 0)
    load_program("Program 1", prog1, inv);
  else if (mode1 == 0 && mode2 == 1)
    load_program("Program 3", prog3, inv);
  else
    load_program("Program 2", prog2, inv);
}

/*
unio
*/

// bool cycle = false;
void loop()
{
  // driver.set_speed(80, 80);
  robot.loop(program);
  // digitalWrite(LED_BUILTIN , (cycle = !cycle)); // Test LED for poling rate
  delay_us = 2000 - (micros() - time);
  delay(DELAY - 2);
}
