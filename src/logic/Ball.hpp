#pragma once
#include <iostream>

#include "GamePad.hpp"

using std::cout;
using std::endl;

class Ball {

public:
    int radius;
    int centerX;
    int centerY;
    float velocityX;
    float velocityY;
    float speedScale = 5;
    double lastTime = 0;
    float airResistance = 0.05;
    std::pair<int, int> xSpeedRange = {60, 100}; // x,y
    std::pair<int, int> ySpeedRange = {120, 200}; // x,y
    Ball() = default;

    Ball(const int _centerX, const int _centerY) : radius(7) {
        reset(_centerX, _centerY);
    }

    void reset(const int _centerX, const int _centerY) {
        centerX = _centerX;
        centerY = _centerY;

        const int dir = GetRandomValue(0, 3);
        velocityX = static_cast<float>((((1 & dir) << 1) - 1) *
                                       GetRandomValue(xSpeedRange.first,
                                                      xSpeedRange.second)) *
                    speedScale;
        velocityY = static_cast<float>((((dir >> 1) << 1) - 1) *
                                       GetRandomValue(ySpeedRange.first,
                                                      ySpeedRange.second)) *
                    speedScale;

        lastTime = GetTime();
    }

    void draw() const {
        // cout << "pos" << centerX << " " << centerY << endl;
        DrawCircle(centerX + GamePad::margin, centerY + GamePad::margin,
                   static_cast<float>(radius), BLACK);
    }

    void update(const double time) {
        centerX += static_cast<int>(velocityX * (time - lastTime));
        centerY += static_cast<int>(velocityY * (time - lastTime));
        if (std::abs(velocityX) > static_cast<float>(xSpeedRange.second) * 5) {
            velocityX *= 1 - airResistance;
        } else if (std::abs(velocityX) >
                   static_cast<float>(xSpeedRange.second) * 3) {
            velocityX *= 1 - airResistance / 2;
        }
        if (std::abs(velocityY) > static_cast<float>(ySpeedRange.second) * 5) {
            velocityY *= 1 - airResistance;
        } else if (std::abs(velocityY) >
                   static_cast<float>(ySpeedRange.second) * 3) {
            velocityY *= 1 - airResistance / 2;
        }
        lastTime = time;
    }
};
