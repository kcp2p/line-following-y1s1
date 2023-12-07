#ifndef LINE_FOLLOWER_H
# define LINE_FOLLOWER_H

#include <FastPID.h>
#include "include/env.hpp"
#include "include/SensorArray.hpp"
#include "include/MortorDriver.hpp"
#include "include/Gripper.hpp"

#define SAFTEY_CYCLE 600

enum e_cmd  {
  _end,
  _begin,
  _right,
  _abs_right,
  _left,
  _abs_left,
  _fast,
  _mid,
  _slow,
  _grab,
  _throw,
  _delay,
  _stop,
  _goto
};

class Robot {
  public:
    Robot (SensorArray *sensor, FastPID *controller, DriverL298N *driver, Gripper *gripper);
    void loop(enum e_cmd program[MAX_PROG][2]);
    int  keep_track(int speed);
    void log(char *str);
    void next ();
    void check();
    void check_intersect(enum e_cmd arg, int steer, bool left_side, bool right_side);
  
  private:
    SensorArray *_sensors;
    FastPID     *_controller;
    DriverL298N *_driver;
    Gripper     *_gripper;
    bool        _on_intersection;
    bool        _grabbed;
    int         _line;
    int         _intersection;
    int         _printed;
};

#endif
