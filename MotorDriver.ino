// Driver controller lib

/*
DriverL298N Contructor

@param int MotorA[3]
@param int MotorB[3]
@param int min
@param int max

- init OUTPUT pins for the motor control wire
*/
DriverL298N::DriverL298N(int MotorA[3], int MotorB[3], int min, int max)
{
  for (int i = 0;  i < 3; i++)
  {
    pinMode(MotorA[i], OUTPUT);
    pinMode(MotorB[i], OUTPUT);
    _MotorA[i] = MotorA[i];
    _MotorB[i] = MotorB[i];
  }
  _min = min;
  _max = max;
}

/*
DriverL298N::set_speed
@param int A (range: -255 to 255)
@param int B (range: -255 to 255)

- set speed for both motor 
*/
void DriverL298N::set_speed(int A, int B)
{
  int _A = abs(A);
  int _B = abs(B);
  if (_A > _max)
    _A = _max;
  else if (_A < _min)
    this->stop_A();
  if (_B > _max)
    _B = _max;
  else if (_B < _min)
    this->stop_B();
  analogWrite(_MotorA[0], _A);
  analogWrite(_MotorB[0], _B);
  digitalWrite(_MotorA[1], A > 0 ? HIGH : LOW);
  digitalWrite(_MotorA[2], A > 0 ? LOW : HIGH);
  digitalWrite(_MotorB[1], B > 0 ? HIGH : LOW);
  digitalWrite(_MotorB[2], B > 0 ? LOW : HIGH);
}

/*
DriverL298N::stop_A

stop motorA
*/
void DriverL298N::stop_A()
{
  analogWrite(_MotorA[0], 0);
  digitalWrite(_MotorA[1], LOW);
  digitalWrite(_MotorA[2], LOW);
}

/*
DriverL298N::stop_B

stop motorB
*/
void DriverL298N::stop_B()
{
  analogWrite(_MotorB[0], 0);
  digitalWrite(_MotorB[1], LOW);
  digitalWrite(_MotorB[2], LOW);
}

/*
DriverL298N::stop

stop ! ... and have commit a crime.
in the name of skyrim you need to pay a fine and return all your stolen goods
or face the consequence ! vemin !
*/
void DriverL298N::stop()
{
  this->stop_A();
  this->stop_B();
}
