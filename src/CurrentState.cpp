/*
 * CurrentState.cpp
 *
 *  Created on: 15 maj 2014
 *      Author: MattLech
 */

#include <math.h>

#include "CurrentState.h"

#define POLAR 1

static CurrentState* instance;
long x = 0;
long y = 0;
long r = 0;
long t = 0;
long z = 0;
unsigned int speed = 0;
bool endStopState[3][2];

CurrentState * CurrentState::getInstance() {
   if (!instance) {
      instance = new CurrentState();
   };

   return instance;
};

CurrentState::CurrentState() {
   x = 0;
   y = 0;
   r = 0;
   t = 0;
   z = 0;
   speed = 0;
}

long CurrentState::getX() {
   return x;
}

long CurrentState::getY() {
   return y;
}

long CurrentState::getR() {
   return r;
}

long CurrentState::getT() {
   return t;
}

long CurrentState::getZ() {
   return z;
}

long* CurrentState::getPoint() {
   long currentPoint[3] = {x, y, z};

   return currentPoint;
}

long* CurrentState::rectToPolar(long rect[]) {
   static long *polar = new long[2];

   long x2 = rect[0] - RADIUS_STEPS, y2 = rect[1] - RADIUS_STEPS;

   polar[0] = (long) sqrt(square((double) x2) + square((double) y2)); // r
   if (y2 == 0 && x2 >= 0) polar[1] = 0; // t
   else if (y2 == 0) polar[1] = 180;
   else if (x2 == 0 && y2 > 0) polar[1] = 90;
   else if (x2 == 0) polar[1] = 270;
   else {
      polar[1] = (long) (atan((double) y2 / x2) * 180 / M_PI);
      if (x2 < 0) polar[1] += 180; // quadrant correction
      else if (y2 < 0) polar[1] += 360;
   }
   polar[1] = (long) (polar[1] * STEPS_PER_DEG); // degs to steps

   return polar;
}

long* CurrentState::polarToRect(long polar[]) {
   static long *rect = new long[2];

   long r2 = polar[0];
   double t2 = polar[1] / STEPS_PER_DEG * M_PI / 180;

   rect[0] = (long) (r2 * cos(t2)) + RADIUS_STEPS; // x
   rect[1] = (long) (r2 * sin(t2)) + RADIUS_STEPS; // y

   return rect;
}

void CurrentState::setX(long newX) {
   long *rect = new long[2];
   long *polar;

   x = newX;
   rect[0] = x;
   rect[1] = y;
   polar = rectToPolar(rect);
   r = polar[0];
   t = polar[1];
}

void CurrentState::setY(long newY) {
   long *rect = new long[2];
   long *polar;

   y = newY;
   rect[0] = x;
   rect[1] = y;
   polar = rectToPolar(rect);
   r = polar[0];
   t = polar[1];
}

void CurrentState::setR(long newR) {
   long *polar = new long[2];
   long *rect;

   r = newR;
   polar[0] = r;
   polar[1] = t;
   rect = polarToRect(polar);
   x = rect[0];
   y = rect[1];
}

void CurrentState::setT(long newT) {
   long *polar = new long[2];
   long *rect;

   t = newT;
   polar[0] = r;
   polar[1] = t;
   rect = polarToRect(polar);
   x = rect[0];
   y = rect[1];
}

void CurrentState::setZ(long newZ) {
   z = newZ;
}

void CurrentState::setEndStopState(unsigned int axis, unsigned int position, bool state) {
   endStopState[axis][position] = state;
}

void CurrentState::storeEndStops() {
   CurrentState::getInstance()->setEndStopState(0,0,digitalRead(X_MIN_PIN));
   CurrentState::getInstance()->setEndStopState(0,1,digitalRead(X_MAX_PIN));
   CurrentState::getInstance()->setEndStopState(1,0,digitalRead(Y_MIN_PIN));
   CurrentState::getInstance()->setEndStopState(1,1,digitalRead(Y_MAX_PIN));
   CurrentState::getInstance()->setEndStopState(2,0,digitalRead(Z_MIN_PIN));
   CurrentState::getInstance()->setEndStopState(2,1,digitalRead(Z_MAX_PIN));
}

void CurrentState::printPosition() {
   Serial.print("R82");
   Serial.print(" X");
   Serial.print(x);
   Serial.print(" Y");
   Serial.print(y);
   Serial.print(" R");
   Serial.print(r);
   Serial.print(" T");
   Serial.print(t);
   Serial.print(" Z");
   Serial.print(z);
   Serial.print("\n");
}

void CurrentState::printBool(bool value)
{
   if (value)
   {
      Serial.print("1");
   }
   else
   {
      Serial.print("0");
   }
}

void CurrentState::printEndStops() {
   Serial.print("R81");
   Serial.print(" XA");
   printBool(endStopState[0][0]);
   Serial.print(" XB");
   printBool(endStopState[0][1]);
   Serial.print(" YA");
   printBool(endStopState[1][0]);
   Serial.print(" YB");
   printBool(endStopState[1][1]);
   Serial.print(" ZA");
   printBool(endStopState[2][0]);
   Serial.print(" ZB");
   printBool(endStopState[2][1]);
   Serial.print("\n");
}

void CurrentState::print() {
   printPosition();
   printEndStops();
}
