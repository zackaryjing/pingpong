#pragma once

#define WIN32_LEAN_AND_MEAN
#define NOGDI
#include <iostream>
#include "Plate.hpp"
#include "Ball.hpp"
#include "raylib.h"

using std::cout;
using std::endl;

class Board {
public:
    int outerWidth;
    int outerHeight;
    int innerWidth;
    int innerHeight;
    int borderThick = 10;
    int marginHeight = 100;
    Vector2 mousePosOld;
    Vector2 mousePos;
    float vTransRatio = 0.5;

    Plate otherPlate;
    Plate selfPlate;
    int otherPlateBaseY;
    int selfPlateBaseY;
    int selfPlateUpperBound;
    int selfPlateLowerBound;
    int selfPlateBoundHeight = 50;
    Ball ball;
    Board() = default;
    Rectangle border;

    Board(int _outerWidth, int _outerHeight) :
        outerHeight(_outerHeight), outerWidth(_outerWidth) {
        innerWidth = outerWidth - borderThick;
        innerHeight = outerHeight - borderThick;

        selfPlateBaseY = outerHeight - borderThick - marginHeight;
        otherPlateBaseY = borderThick + marginHeight;
        selfPlateUpperBound = selfPlateBaseY - selfPlateBoundHeight;
        selfPlateLowerBound = selfPlateBaseY + selfPlateBoundHeight;
        border = Rectangle(0.0f + GamePad::margin,
                           0.0f + GamePad::margin,
                           static_cast<float>(outerWidth),
                           static_cast<float>(outerHeight));
        otherPlate = Plate(outerWidth / 2, otherPlateBaseY,
                           PlatePosType::TopCenter);
        selfPlate = Plate(outerWidth / 2,
                          selfPlateBaseY,
                          PlatePosType::BottomCenter);
        ball = Ball(outerWidth / 2, outerHeight / 2);
    }

    void checkCollision() {
        const auto ballUpper = ball.centerY - ball.radius;
        const auto ballLower = ball.centerY + ball.radius;
        const auto halfHeight = selfPlate.height / 2;
        const auto pastTime = GetFrameTime();
        // side wall
        if (ball.centerX + ball.radius > innerWidth) {
            ball.velocityX *= -0.7;
            ball.velocityY *= 0.9;
            ball.centerX = innerWidth - ball.radius - 1;
        }
        if (ball.centerX - ball.radius < borderThick) {
            ball.velocityX *= -0.7;
            ball.velocityY *= 0.9;
            ball.centerX = borderThick + ball.radius + 1;
        }


        // otherPlate collision
        if (ballUpper < otherPlate.y + otherPlate.height) {
            ball.velocityY = -ball.velocityY;
            ball.velocityX += otherPlate.getHorizontalSpeed(pastTime) *
                    vTransRatio;
        }

        // selfPlate collision
        if (ball.centerX > selfPlate.x - ball.radius &&
            ball.centerX < selfPlate.x + selfPlate.width + ball.radius) {
            if (ball.centerX > selfPlate.x &&
                ball.centerX < selfPlate.x + selfPlate.width &&
                ballLower > selfPlate.y &&
                ballLower < selfPlate.y + selfPlate.height) {
                // top collide
                ball.velocityY = -ball.velocityY;
                ball.velocityX += selfPlate.getHorizontalSpeed(pastTime) *
                        vTransRatio;
            } else if (ball.centerY > selfPlate.y - ball.radius &&
                       ball.centerY < selfPlate.y + ball.radius) {
                // side collide
                // ball.velocityY =
                ball.velocityX =
                        selfPlate.getHorizontalSpeed(pastTime) * (
                            1 + vTransRatio);

            }
        }


        // out of boundary
        if (ballLower > innerHeight || ballUpper < borderThick) {
            ball.reset(outerWidth / 2, outerHeight / 2);
        }
    }

    void update(const double time) {
        mousePos = GetMousePosition();
        const int mouseX = static_cast<int>(mousePos.x) - GamePad::margin;
        const int mouseY = static_cast<int>(mousePos.y) - GamePad::margin;
        int selfPlateY;
        if (mouseY < selfPlateBaseY) {
            selfPlateY = selfPlateBaseY - selfPlateBoundHeight * (
                             selfPlateBaseY - mouseY)
                         / (selfPlateBaseY - mouseY +
                            selfPlateBoundHeight);
        } else {
            selfPlateY = selfPlateBaseY + selfPlateBoundHeight * (
                             mouseY - selfPlateBaseY)
                         / (mouseY - selfPlateBaseY +
                            selfPlateBoundHeight);

        }

        selfPlate.update(mouseX, selfPlateY);
        otherPlate.update(ball.centerX - otherPlate.width / 2, otherPlateBaseY);
        checkCollision();
        ball.update(time);
    }

    void draw() {
        DrawRectangleLinesEx(border, 10.0, GRAY);
        otherPlate.draw();
        selfPlate.draw();
        ball.draw();
    }
};
