#ifndef GRIPPER_H
# define GRIPPER_H
#include <Servo.h>

class Gripper {
  public:
    Gripper (int gripper_pin, int arm_pin);
    void    set_zero();
    void    detach();
    void    lower ();
    void    retract ();
    void    open();
    void    close();
  private:
    int _gripper_pin;
    int _arm_pin;
    Servo _gripper_servo;
    Servo _arm_servo;
};

#endif
