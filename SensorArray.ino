/*
SensorArray Construcotor
@param int* pins
@param int threshold

- set pinMode
- save pin and thresold into class attrs

*/
SensorArray::SensorArray(int *pins, int threshold)
{
  // get number of pins
  _n_pins = 0;
  while (pins[_n_pins] != -1)
  {
    pinMode(pins[_n_pins], INPUT);
    _n_pins++;
  }
  // Construct pins
  _pins = new int[_n_pins];
  for (int i = 0; i < _n_pins; i++)
    _pins[i] = pins[i];
  // Set threshold
  _threshold = threshold;
  _reading = 0;
}

/*

SensorArray Deconstructor

*/
SensorArray::~SensorArray()
{
  delete _pins;
}

// int     fake_analogRead(int pin)
// {
//   switch (pin)
//   {
//     case A1: return 0.0 * 1023; break;
//     case A2: return 0.0 * 1023; break;
//     case A3: return 0.0 * 1023; break;
//     case A4: return 0.0 * 1023; break;
//     case A5: return 0.0 * 1023; break;
//   }
// }

/*
SensorArray::get_raw
@param int pin
@param bool as_digital

check pin reading as digital or analog base on as_digital flag
*/
SensorArray::get_raw(int pin, bool as_digital)
{
  if (pin == -1)
    pin = _n_pins - 1;
  if (as_digital)
    return (float)((digitalRead(_pins[pin]) << 10) - 1);
  return analogRead(_pins[pin]);
}

/*
SensorArray::read
@param bool as_digital

@see https://www.cuemath.com/weighted-mean-formula/

calculate the avg sensor reading from all the sensor
and return if the line bias value 

*/
int16_t SensorArray::read(bool as_digital)
{
  float value = 0, sum = 0, reading;
  // Serial.print("Sensor Array:");
  for (int i = 0; i < _n_pins; i++)
  {
    int weight  = (i * 2046 / (_n_pins - 1)) - 1023;
    reading = this->get_raw(i, as_digital);
    // if (i == 1)
    //   reading = reading + 175;
    if (reading < THRESH)
      reading = 0;
    value += reading * weight;
    sum += reading;
    Serial.print(" ");
    // if (reading)
    //   Serial.print("O");
    // else
    //   Serial.print("-");
    Serial.print(reading);
  }
  Serial.print("; ");
  if (!sum)
  {
    Serial.println("--No line");
    _on_line = false;
    return _reading;
  }
  _on_line = true;
  _reading = value / sum;
  Serial.println(_reading);
  return _reading;
}

/*
 Beep Boop on line ?
*/
bool SensorArray::on_line()
{
  return _on_line;
}

/*

@param status
Beep Boop on_line is status ?

*/
bool SensorArray::on_line(bool status)
{
  return (_on_line = status);
}
