/*
 * CurrentState.h
 *
 *  Created on: 15 maj 2014
 *      Author: MattLech
 */

#ifndef CURRENTSTATE_H_
#define CURRENTSTATE_H_
#include "Arduino.h"
#include "pins.h"

#define RADIUS_STEPS 6000
// #define STEPS_PER_DEG 14.9173553 // 27:1
// #define STEPS_PER_DEG 28.5714286 // 51:1
#define STEPS_PER_DEG 55.5555556 // 100:1

class CurrentState {
public:
   static CurrentState* getInstance();
   long getX();
   long getY();
   long getR();
   long getT();
   long getZ();
   long* getPoint();
   static long* rectToPolar(long rect[]);
   static long* polarToRect(long polar[]);
   void setX(long);
   void setY(long);
   void setR(long);
   void setT(long);
   void setZ(long);
   void setEndStopState(unsigned int, unsigned int, bool);
   void printPosition();
   void storeEndStops();
   void printEndStops();
   void print();
   void printBool(bool);
private:
   CurrentState();
   CurrentState(CurrentState const&);
   void operator=(CurrentState const&);
};

#endif /* CURRENTSTATE_H_ */
