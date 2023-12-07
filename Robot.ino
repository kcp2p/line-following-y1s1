#include "line_follower.hpp"

/*
Robot class
@param SensorArray* sensor
@param FastPID* controller
@param DriverL298N* driver
*/

int slow_cycling = 0;
int cycle_count = 0;

Robot::Robot (SensorArray *sensor, FastPID *controller, DriverL298N *driver, Gripper *gripper)
{
  va_list args;
  _sensors = sensor;
  _controller = controller;
  _driver = driver;
  _gripper = gripper;
  _line = 0;
  _intersection = 0;
  _on_intersection = true;
  _printed = false;
  _grabbed = false;
  _sensors->on_line(true);
}

/*
Robot::next

call next instruction set base on pre-determined enum value
*/
void Robot::next()
{
  _line++;
  _printed = false;
  _intersection = 0;
  Serial.print("[line ");
  Serial.print(_line);
  Serial.print("] ");
  time = micros();
}

/*
Robot::check

check for intersection and turn on intersect flag 
*/
void Robot::check()
{
  _intersection++;
  _on_intersection = true;
  Serial.print("Intersection ");
  Serial.print(_intersection);
  Serial.println(" reached!");
  driver.stop();
  delay(1000);
  digitalWrite(LED_BUILTIN, HIGH);
  driver.set_speed(MAX_SPEED, MAX_SPEED);
  delay(200);
  time = micros();
}

/*
Robot::keep_track

read analog signal from sensor as steer and try to keep steer straight
*/
int Robot::keep_track(int speed)
{
  // Reduced measurment interval from around +1420μs (142%) to +20μs (2%) error
  if (time)
    delayMicroseconds(delay_us - 450); // Delay for the remaining microsecond.
  // Serial.print(micros() - time - (DELAY * 1000UL));
  // Serial.println("μs latency.");
  time = micros(); // Set time start for precise delay
  // sensors.read(); -> range -1024 to 1023
	int steer = _controller->step(0, _sensors->read(false)); // steer range: -1024 to 1023
  // reduce to steer -255 to 255
  if (steer < 0)
    steer = - (-steer >> 2);
  else
    steer = steer >> 2;
  _driver->set_speed(speed - steer, speed + steer);
  return steer;
}

/*
Robot::log

log the fuck kub
*/
void Robot::log(char *str)
{
  if (!_printed)
    Serial.println(str);
  _printed = true;
}

/*
Robot::check_intersect

check if robot is at intersect
*/
void Robot::check_intersect(enum e_cmd arg, int steer, bool left_side, bool right_side)
{
  if (_sensors->on_line())
  {
    cycle_count = 0;
    if (!_on_intersection && abs(steer) < 100 && (left_side || right_side))
      this->check();
    if (_intersection == arg)
      this->next();
  } else {
    cycle_count += 1;
    if (cycle_count >= SAFTEY_CYCLE) {
      this->_driver->stop();
    }
  }
}

/*
Robot::loop

loopa roboto exexcushon (with French accent). 
- get sensor reading
- check step against instruction set
- check intersection
*/
void Robot::loop(enum e_cmd program[MAX_PROG][2])
{
  const int cmd = program[_line][0];
  const int arg = program[_line][1];
  int       steer;
  bool      left_side, right_side;

  left_side = _sensors->get_raw(0, false) + _sensors->get_raw(1, false) + _sensors->get_raw(2, false) > LINE_THRESH;
  right_side = _sensors->get_raw(2, false) + _sensors->get_raw(3, false) + _sensors->get_raw(4, false) > LINE_THRESH;
  char buff[99];
  sprintf(buff, "left_side: %d, right_side: %d %d %d %d", left_side, right_side, _sensors->get_raw(0, false), _sensors->get_raw(4, false), LINE_THRESH);
  // Serial.println(buff);
  switch (cmd)
  {
    case _begin:
      cycle_count = 0;
      this->log("----");
      _driver->set_speed(MAX_SPEED, MAX_SPEED);
      delay(100);
      _on_intersection = false;
      this->next();
    break;
    case _fast:
      this->log("Forward [fast]");
      steer = this->keep_track(MAX_SPEED);
      this->check_intersect(arg, steer, left_side, right_side);
    break;
    case _mid:
      this->log("Forward [medium]");
      steer = this->keep_track(MID_SPEED);
      this->check_intersect(arg, steer, left_side, right_side);
    break;
    case _slow:
      this->log("Slow down... Jai yen yen.");
      steer = this->keep_track(MIN_SPEED);
      this->check_intersect(arg, steer, left_side, right_side);
    break;
    case _right:
      this->log("Turning right...");
      _driver->set_speed(MAX_SPEED, -MAX_SPEED);
      delay(140);
      while (_sensors->get_raw(2, false) + _sensors->get_raw(3, false) < THRESH) ;
      _driver->set_speed(-MAX_SPEED, MAX_SPEED);
      delay(10);
      this->next();
    break;
    case _abs_right:
      this->log("Turning absolute right...");
      _driver->set_speed(255, -255);
      delay(480);
      this->next();  
    break;
    case _left:
      this->log("Turning left...");
      _driver->set_speed(-MAX_SPEED, MAX_SPEED);
      delay(140);
      while (_sensors->get_raw(1, false) + _sensors->get_raw(2, false) < THRESH) ;
      _driver->set_speed(MAX_SPEED, -MAX_SPEED);
      delay(10);
      this->next();
    break;
    case _abs_left:
      this->log("Turning absolute left...");
      _driver->set_speed(-255, 255);
      delay(480);
      this->next();  
    break;
    case _grab:
      this->log("Grabbing the cube...");
      // _driver->stop();
      // _gripper->open();
      // _gripper->lower();
      // while (!_sensors->get_raw(0, true) && !_sensors->get_raw(0, true))
      // {
      //   this->keep_track(MIN_SPEED);
      //   delay(DELAY);
      // }
      // _gripper->close();
      // _gripper->retract();
      // _driver->set_speed(-MAX_SPEED, -MAX_SPEED);
      // delay(300);
      this->next();
    break;
    case _end:
      this->log("Parking...");
      Serial.println("> Find the finishing point...");
      delay_us = 0;
      while (_sensors->on_line() && !_sensors->get_raw(0, true))
      {
        this->keep_track(MIN_SPEED);
        delay(DELAY);
      }
      Serial.println("> Finishing point reached.");
      _driver->stop();
      // _gripper->detach();
      Serial.println("Program Terminated.");
      for (;;) {
        digitalWrite(LED_BUILTIN, LOW);
        delay(500);
        digitalWrite(LED_BUILTIN, HIGH);
        delay(500);
      }
    break;
    case _delay:
      this->log("Delay...");
      delay(arg);
      this->next();
    break;
    case _stop:
      _driver->stop();
      Serial.println("Stopped.");
      for (;;) {
        digitalWrite(LED_BUILTIN, LOW);
        delay(500);
        digitalWrite(LED_BUILTIN, HIGH);
        delay(300);
      }
    break;
    case _goto:
      this->log("Jump line instruction.");
      _line = arg;
      this->next();
    break;
    default:
      _driver->stop();
      Serial.print("Program Error: Invalid command. ");
      for (;;) {
        digitalWrite(LED_BUILTIN, LOW);
        delay(400);
        digitalWrite(LED_BUILTIN, HIGH);
        delay(100);
      }
    break;
  }
  if (_on_intersection && !left_side && !right_side)
    _on_intersection = false;
}
