/*****************************************************************
 * Copyright (c) 2020 CodeAbove.
 * All rights reserved.
 *
 * This software component is licensed by CodeAbove under
 * BSD 3-Clause license, the 'License'. You may not use
 * this file except in compliance with the License. You
 * may obtain a copy of the License at:
 *                  opensource.org/licenses/BSD-3-Clause
 ****************************************************************/
#ifndef LASER_H
#define LASER_H

#include <modm/board.hpp>
#include <modm/platform/timer/basic_base.hpp>
#include <modm/platform/gpio/base.hpp>

template<typename TIMER>
class Laser
{
public:
    Laser(void) = delete;
    Laser(uint8_t channel) : _channel(channel), _brightness(100) {}

    inline void setBrightness(uint8_t brightness)
    {
        _brightness = brightness;
        TIMER::setCompareValue(_channel, _brightness);
    }

    inline void setOff(void)
    {
        setBrightness(0);
    }

    inline void setOn(void)
    {
        setBrightness(100);
    }

private:
    uint8_t _channel;
    uint8_t _brightness;
};

#endif // LASER_H
