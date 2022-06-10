/*****************************************************************
 * Copyright (c) 2022 CodeAbove.
 * All rights reserved.
 *
 * This software component is licensed by CodeAbove under
 * BSD 3-Clause license, the 'License'. You may not use
 * this file except in compliance with the License. You
 * may obtain a copy of the License at:
 *                  opensource.org/licenses/BSD-3-Clause
 ****************************************************************/
#ifndef SERVO_H
#define SERVO_H

#include <modm/board.hpp>
#include <modm/platform/gpio/base.hpp>
#include <modm/platform/timer/basic_base.hpp>

template<typename TIMER>
class Servo
{
public:
    Servo(void) = delete;
    Servo(uint8_t channel, uint16_t dutyCycle, uint16_t minDutyCycle, uint16_t maxDutyCycle)
        : _channel(channel), _dutyCycle(dutyCycle), _minDutyCycle(minDutyCycle), _maxDutyCycle(maxDutyCycle) {}

    inline void setDutyCycle(uint16_t dc)
    {
        if(dc < _minDutyCycle)
        {
            _dutyCycle = _minDutyCycle;
        }
        else if(dc > _maxDutyCycle)
        {
            _dutyCycle = _maxDutyCycle;
        }
        else 
        {
            _dutyCycle = dc;
        }

        TIMER::setCompareValue(_channel, _dutyCycle);
    }

    void center(void)
    {
        setDutyCycle(_minDutyCycle+((_maxDutyCycle-_minDutyCycle)/2));
    }

    uint16_t getDutyCycle(void)
    {
        return _dutyCycle;
    }

private:
    uint8_t _channel;

    uint16_t _dutyCycle;
    uint16_t _minDutyCycle;
    uint16_t _maxDutyCycle;
};

#endif // SERVO_H
