#ifndef DRIVER_L298N_H
# define  DRIVER_L298N_H

class DriverL298N
{
  public:
    DriverL298N(int MotorA[3], int MotorB[3], int min, int max);
    void set_speed(int A, int B);
    void stop();
    void stop_A();
    void stop_B();
  private:
    int _MotorA[3];
    int _MotorB[3];
    int _min;
    int _max;
};

#endif
