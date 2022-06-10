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
#ifndef MOTION_H
#define MOTION_H

#include "servo.h"

#include <stdint.h>

using coordinate_t = uint32_t;

struct Point
{
    coordinate_t x;
    coordinate_t y;

    Point(coordinate_t x, coordinate_t y) : x(x), y(y) { };

    Point operator+(const Point &rhs)
    {
        return Point(x+rhs.x, y+rhs.y);
    }

    Point operator+=(const Point &rhs)
    {
        *this = *this + rhs;
        return *this;
    }
};

template<typename TIMER>
class Motion
{
    enum class State
    {
        IDLE,
        HOLD,
        MOVE
    } state;

    Servo<TIMER>& servoX;
    Servo<TIMER>& servoY;


    uint16_t interval;

    coordinate_t dx;
    coordinate_t dy;
    uint16_t steps;


    void setMicroSteps(uint16_t time);
    void moveTo(Point position);

public:
    Point current_pos;
    Point target_pos;
    Motion(Servo<TIMER>& servoX, Servo<TIMER>& servoY, uint16_t interval)
    : servoX(servoX), servoY(servoY), current_pos({0,0}), target_pos({0,0}), interval(interval)
    { 
        state = State::IDLE;
    };

    void Initialize(void);
    void Perform(void);
    void SetTargetPosition(Point position, uint16_t time);

};

template<typename TIMER>
void Motion<TIMER>::Initialize(void)
{
    servoX.center();
    current_pos.x = servoX.getDutyCycle();

    servoY.center();
    current_pos.y = servoY.getDutyCycle();
}

template<typename TIMER>
void Motion<TIMER>::Perform(void)
{
    switch(state)
    {
    case State::IDLE:

        break;

    case State::HOLD:

        break;

    case State::MOVE:
        if(steps > 0)
        {
            Point next_position { current_pos.x + dx, current_pos.y + dy };
            moveTo(next_position);
            steps--;
        }
        else
        {
            moveTo(target_pos);
            state = State::HOLD;
        }
        break;

    default:
        state = State::HOLD;
        break;
    }
}

template<typename TIMER>
void Motion<TIMER>::SetTargetPosition(Point position, uint16_t time)
{
    target_pos = position;
    setMicroSteps(time);

    state = State::MOVE;
} 

template<typename TIMER>
void Motion<TIMER>::setMicroSteps(uint16_t time)
{
    steps = time / interval;
    dx = (target_pos.x - current_pos.x) / steps;
    dy = (target_pos.y - current_pos.y) / steps;
}

template<typename TIMER>
void Motion<TIMER>::moveTo(Point position)
{
    current_pos = position;
    servoX.setDutyCycle(current_pos.x);
    servoY.setDutyCycle(current_pos.y);
}

#endif // MOTION_H
