#ifndef SENSOR_ARRAY_H
# define  SENSOR_ARRAY_H
# include "env.hpp"

class SensorArray
{
	public:
		SensorArray (int *pins, int threshold);
    ~SensorArray();

    int16_t read(bool as_digital);
    int16_t get_raw(int pin, bool as_digital);
    bool    on_line();
    bool    on_line(bool status);
  private:
    int   *_pins;
    int   _n_pins;
    int   _threshold;
    int   _reading;
    bool  _on_line;
};

#endif
