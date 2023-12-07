#include "include/Gripper.hpp"

Gripper:: Gripper (int gripper_pin, int arm_pin)
{
  _gripper_pin = gripper_pin;
  _arm_pin = arm_pin;
}

// speed range 1 - 100
void Gripper::lower ()
{
  Serial.println("> Lowering servo...");
  // _arm_servo.attach(_arm_pin);
  for (int i = 0; i < 110; i++)
  {
    _arm_servo.write(i);
    delay(2);
  }
  delay(600);
  // _arm_servo.detach();
}

void Gripper::retract ()
{
  Serial.println("> Retracting servo...");
  // _arm_servo.attach(_arm_pin);
  for (int i = 110; i > 0; i--)
  {
    _arm_servo.write(i);
    delay(2);
  }
  // _arm_servo.detach();
}

void Gripper::open ()
{
  Serial.println("> Openning servo...");
  // _gripper_servo.attach(_gripper_pin);
  _gripper_servo.write(40);
  // _gripper_servo.detach();
}

void Gripper::close ()
{
  Serial.println("> Closing servo...");
  // _gripper_servo.attach(_gripper_pin);
  _gripper_servo.write(0);
  delay(600);
  // _gripper_servo.detach();
}

void Gripper::set_zero()
{
  Serial.println("Seting zero servo...");
  _gripper_servo.attach(_gripper_pin);
  _arm_servo.attach(_arm_pin);
  _gripper_servo.write(0);
  _arm_servo.write(0);
  delay(1000);
}

void Gripper::detach()
{
  Serial.println("Detaching servo...");
  this->set_zero();
  Serial.println("Servo detached.");
}
