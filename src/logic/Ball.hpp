#pragma once
#include <iostream>

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
    std::pair<int, int> xSpeedRange = {20, 40}; // x,y
    std::pair<int, int> ySpeedRange = {60, 80}; // x,y
    Ball() = default;

    Ball(const int _centerX, const int _centerY) :
        radius(7) {
        reset(_centerX, _centerY);
    }

    void reset(const int _centerX, const int _centerY) {
        centerX = _centerX;
        centerY = _centerY;

        const int dir = GetRandomValue(0, 3);
        velocityX = static_cast<float>((((1 & dir) << 1) - 1)
                                       * GetRandomValue(
                                               xSpeedRange.first,
                                               xSpeedRange.second)) *
                    speedScale;
        velocityY = static_cast<float>((((dir >> 1) << 1) - 1) *
                                       GetRandomValue(
                                               ySpeedRange.first,
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
        lastTime = time;
    }
};
