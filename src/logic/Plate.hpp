#pragma once
#include "GamePad.hpp"

enum class PlatePosType {
    TopCenter,
    BottomCenter,
    TopLeft,
};

class Plate {
public:
    int x;
    int y;
    int oldX;
    int oldY;
    int centerX;
    int centerY;
    int height = 10;
    int width = 60;
    Plate() = default;

    Plate(const int _centerX, const int _centerY) :
        centerX(_centerX), centerY(_centerY) {
        x = centerX - width / 2;
        y = centerY - height / 2;
    }

    Plate(const int _x, const int _y, const PlatePosType posType) {
        if (posType == PlatePosType::BottomCenter) {
            centerX = _x;
            centerY = _y - height / 2;
            x = _x - width / 2;
            y = _y - height;
        } else if (posType == PlatePosType::TopCenter) {
            centerX = _x;
            centerY = _y + height / 2;
            x = _x - width / 2;
            y = _y;
        } else if (posType == PlatePosType::TopLeft) {
            centerX = _x + width / 2;
            centerY = _y + height / 2;
            x = _x;
            y = _y;
        }
    }


    void update(const int _x, const int _y) {
        oldX = x;
        oldY = y;
        x = _x;
        y = _y;
    }

    [[nodiscard]] float getHorizontalSpeed(const float frameTime) const {
        return static_cast<float>(x - oldX) / frameTime;
    }

    [[nodiscard]] float getVerticalSpeed(const float frameTime) const {
        return static_cast<float>(y - oldY) / frameTime;
    }

    int getVerticalDis() const { return y - oldY; }

    void draw() {
        DrawRectangle(x + GamePad::margin, y + GamePad::margin, width, height,
                      BLACK);
    }
};
