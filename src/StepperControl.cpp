#include "StepperControl.h"
#include "CurrentState.h"
#include "TimerOne.h"

#define POLAR 1

static StepperControl* instance;

StepperControl * StepperControl::getInstance() {
   if (!instance) {
      instance = new StepperControl();
   };
   return instance;
}
;

const int MOVEMENT_INTERRUPT_SPEED = 100; // Interrupt cycle in micro seconds

StepperControl::StepperControl() {

   axis1 = StepperControlAxis();
   axis2 = StepperControlAxis();
   axis3 = StepperControlAxis();

   if (POLAR) {
      axis1.label = 'R';
      axis2.label = 'T';
   } else {
      axis1.label = 'X';
      axis2.label = 'Y';
   }
   axis3.label = 'Z';

   axis1.loadPinNumbers(X_STEP_PIN, X_DIR_PIN, X_ENABLE_PIN, X_MIN_PIN, X_MAX_PIN);
   axis2.loadPinNumbers(Y_STEP_PIN, Y_DIR_PIN, Y_ENABLE_PIN, Y_MIN_PIN, Y_MAX_PIN);
   axis3.loadPinNumbers(Z_STEP_PIN, Z_DIR_PIN, Z_ENABLE_PIN, Z_MIN_PIN, Z_MAX_PIN);

   loadMotorSettings();
}

/**
 * xDest - destination X in steps
 * yDest - destination Y in steps
 * zDest - destination Z in steps
 * maxStepsPerSecond - maximum number of steps per second
 * maxAccelerationStepsPerSecond - maximum number of acceleration in steps per second
 */
int StepperControl::moveToCoords(
   long xDest, long yDest, long zDest, unsigned int xMaxSpd,
   unsigned int yMaxSpd, unsigned int zMaxSpd, bool xHome, bool yHome,
   bool zHome
) {
   unsigned long currentMillis = 0;
   unsigned long timeStart = millis();

   int incomingByte     = 0;
   int error            = 0;

   // load motor settings

   loadMotorSettings();

   // if a speed is given in the command, use that instead of the config speed

   if (xMaxSpd > 0 && xMaxSpd < speedMax[0]) {
      speedMax[0] = xMaxSpd;
   }

   if (yMaxSpd > 0 && yMaxSpd < speedMax[1]) {
      speedMax[1] = yMaxSpd;
   }

   if (zMaxSpd > 0 && zMaxSpd < speedMax[2]) {
      speedMax[2] = zMaxSpd;
   }

   axis1.setMaxSpeed(speedMax[0]);
   axis2.setMaxSpeed(speedMax[1]);
   axis3.setMaxSpeed(speedMax[2]);

   // Load coordinates into axis class

   long sourcePoint[3]     = {0,0,0};
   if (POLAR) {
      sourcePoint[0] = CurrentState::getInstance()->getR();
      sourcePoint[1] = CurrentState::getInstance()->getT();
   } else {
      sourcePoint[0] = CurrentState::getInstance()->getX();
      sourcePoint[1] = CurrentState::getInstance()->getY();
   }
   sourcePoint[2] = CurrentState::getInstance()->getZ();

   long currentPoint[3]    = {0,0,0};
   if (POLAR) {
      currentPoint[0] = CurrentState::getInstance()->getR();
      currentPoint[1] = CurrentState::getInstance()->getT();
   } else {
      currentPoint[0] = CurrentState::getInstance()->getX();
      currentPoint[1] = CurrentState::getInstance()->getY();
   }
   currentPoint[2] = CurrentState::getInstance()->getZ();

   long destinationPoint[3]= {0,0,0};
   if (POLAR) {
      long *rect = new long[2];
      long *polar;

      rect[0] = xDest;
      rect[1] = yDest;
      polar = CurrentState::rectToPolar(rect);
      destinationPoint[0] = polar[0];
      destinationPoint[1] = polar[1];
   } else {
      destinationPoint[0] = xDest;
      destinationPoint[1] = yDest;
   }
   destinationPoint[2] = zDest;

   // Load coordinates into motor control

   axis1.loadCoordinates(currentPoint[0],destinationPoint[0],xHome);
   axis2.loadCoordinates(currentPoint[1],destinationPoint[1],yHome);
   axis3.loadCoordinates(currentPoint[2],destinationPoint[2],zHome);

   // Prepare for movement

   storeEndStops();
   reportEndStops();


   axis1.setDirectionAxis();
   axis2.setDirectionAxis();
   axis3.setDirectionAxis();

        enableMotors();

   // Start movement

   axisActive[0] = true;
   axisActive[1] = true;
   axisActive[2] = true;


   axis1.checkMovement();
   axis2.checkMovement();
   axis3.checkMovement();

   Timer1.start();

   // Let the interrupt handle all the movements
   while (axisActive[0] || axisActive[1] || axisActive[2]) {

      delay(1);

      axisActive[0] = axis1.isAxisActive();
      axisActive[1] = axis2.isAxisActive();
      axisActive[2] = axis3.isAxisActive();

           storeEndStops();

      // Check timeouts
      if (axisActive[0] == true && ((millis() >= timeStart && millis() - timeStart > timeOut[0] * 1000) || (millis() < timeStart && millis() > timeOut[0] * 1000))) {
         error = 1;
      }
      if (axisActive[1] == true && ((millis() >= timeStart && millis() - timeStart > timeOut[0] * 1000) || (millis() < timeStart && millis() > timeOut[0] * 1000))) {
         error = 1;
      }
      if (axisActive[2] == true && ((millis() >= timeStart && millis() - timeStart > timeOut[0] * 1000) || (millis() < timeStart && millis() > timeOut[0] * 1000))) {
         error = 1;
      }

      // Check if there is an emergency stop command
      if (Serial.available() > 0) {
                  incomingByte = Serial.read();
         if (incomingByte == 69 || incomingByte == 101) {
            error = 1;
         }
           }

      if (error == 1) {
         Serial.print("R99 error\n");
         Timer1.stop();
         axisActive[0] = false;
         axisActive[1] = false;
         axisActive[2] = false;
      }

      // Periodically send message still active
      currentMillis++;
      if (currentMillis % 2500 == 0)
      {
         Serial.print("R04\n");
      }

   }

   Serial.print("R99 stopped\n");

   Timer1.stop();
   disableMotors();

   currentPoint[0] = axis1.currentPoint();
   currentPoint[1] = axis2.currentPoint();
   currentPoint[2] = axis3.currentPoint();

   if (POLAR) {
      CurrentState::getInstance()->setR(currentPoint[0]);
      CurrentState::getInstance()->setT(currentPoint[1]);
   } else {
      CurrentState::getInstance()->setX(currentPoint[0]);
      CurrentState::getInstance()->setY(currentPoint[1]);
   }
   CurrentState::getInstance()->setZ(currentPoint[2]);

   storeEndStops();
   reportEndStops();
   reportPosition();

   return error;
}

//
// Calibration
//

int StepperControl::calibrateAxis(int axis) {

   // Load motor settings

   loadMotorSettings();

   //unsigned long timeStart             = millis();

   bool movementDone    = false;

   int  paramValueInt   = 0;
   int  stepsCount        = 0;
   int  incomingByte    = 0;
   int  error           = 0;


   bool invertEndStops = false;
   int parEndInv;
   int parNbrStp;

   // Prepare for movement

   storeEndStops();
   reportEndStops();


   // Select the right axis
   StepperControlAxis calibAxis;
   switch (axis) {
      case 0:
         calibAxis      = axis1;
         parEndInv      = MOVEMENT_INVERT_ENDPOINTS_X;
         parNbrStp      = MOVEMENT_AXIS_NR_STEPS_X;
         invertEndStops = endStInv[0];
         break;
      case 1:
         calibAxis      = axis2;
         parEndInv      = MOVEMENT_INVERT_ENDPOINTS_Y;;
         parNbrStp      = MOVEMENT_AXIS_NR_STEPS_Y;
         invertEndStops = endStInv[0];
         break;
      case 2:
         calibAxis      = axis3;
         parEndInv      = MOVEMENT_INVERT_ENDPOINTS_Z;
         parNbrStp      = MOVEMENT_AXIS_NR_STEPS_Z;
         invertEndStops = endStInv[0];
         break;
      default:
         Serial.print("R99 Calibration error: invalid axis selected\n");
         return 1;
   }


   // Preliminary checks

   if (calibAxis.endStopMin() || calibAxis.endStopMax()) {
      Serial.print("R99 Calibration error: end stop active before start\n");
      return 1;
   }

   Serial.print("R99");
   Serial.print(" axis ");
   Serial.print(calibAxis.label);
   Serial.print(" calibration start");
   Serial.print("\n");

   // Move towards home

        calibAxis.enableMotor();
   calibAxis.setDirectionHome();

   stepsCount = 0;
   movementDone = false;
   while (!movementDone && error == 0) {

                // Check if there is an emergency stop command
                if (Serial.available() > 0) {
                        incomingByte = Serial.read();
                        if (incomingByte == 69 || incomingByte == 101) {
                                movementDone = true;
            error = 1;
                        }
                }

      // Move until the end stop for home position is reached
      if ((!calibAxis.endStopMin() && !calibAxis.endStopMax()) && !movementDone) {

         calibAxis.setMotorStep();

         delayMicroseconds(1000000 / speedMin[axis] /2);

         stepsCount++;
                   if (stepsCount % (speedMin[axis] * 3) == 0)
                   {
            // Periodically send message still active
                           Serial.print("R04\n");
                   }

         calibAxis.resetMotorStep();
         delayMicroseconds(1000000 / speedMin[axis] /2);

      } else {
         movementDone = true;

         // If end stop for home is active, set the position to zero
         if (calibAxis.endStopMax())
         {
            invertEndStops = true;
         }
      }
   }

   Serial.print("R99");
   Serial.print(" axis ");
   Serial.print(calibAxis.label);
   Serial.print(" at first end stop");
   Serial.print("\n");

   // Report back the end stop setting

   if (error == 0) {
      if (invertEndStops) {
         paramValueInt = 1;
      } else {
         paramValueInt = 0;
      }

      Serial.print("R23");
      Serial.print(" ");
      Serial.print("P");
      Serial.print(parEndInv);
      Serial.print(" ");
      Serial.print("V");
      Serial.print(paramValueInt);
      Serial.print("\n");
   }

   // Store the status of the system

        storeEndStops();
   reportEndStops();

   // Move into the other direction now, and measure the number of steps

   stepsCount = 0;
   movementDone = false;
   calibAxis.setDirectionAway();

   while (!movementDone && error == 0) {

                // Check if there is an emergency stop command
                if (Serial.available() > 0) {
                        incomingByte = Serial.read();
                        if (incomingByte == 69 || incomingByte == 101) {
                                movementDone = true;
            error = 1;
                        }
                }

      // Move until the end stop at the other side of the axis is reached
      if (((!invertEndStops && !calibAxis.endStopMax()) || (invertEndStops && !calibAxis.endStopMin())) && !movementDone) {

         calibAxis.setMotorStep();
         stepsCount++;

         delayMicroseconds(1000000 / speedMin[axis] /2);

                   if (stepsCount % (speedMin[axis] * 3) == 0)
                 {
            // Periodically send message still active
                           Serial.print("R04\n");
                   }


         calibAxis.resetMotorStep();
         delayMicroseconds(1000000 / speedMin[axis] /2);

      } else {
         movementDone = true;
      }
   }


   Serial.print("R99");
   Serial.print(" axis ");
   Serial.print(calibAxis.label);
   Serial.print(" at second end stop");
   Serial.print("\n");

   // Report back the end stop setting

   if (error == 0) {
      Serial.print("R23");
      Serial.print(" ");
      Serial.print("P");
      Serial.print(parNbrStp);
      Serial.print(" ");
      Serial.print("V");
      Serial.print(stepsCount);
      Serial.print("\n");
   }

   calibAxis.disableMotor();

        storeEndStops();
   reportEndStops();


        switch (axis) {
      case 0:
         CurrentState::getInstance()->setX(stepsCount);
         break;
      case 1:
              CurrentState::getInstance()->setY(stepsCount);
         break;
      case 2:
              CurrentState::getInstance()->setZ(stepsCount);
         break;
   }

   reportPosition();

   return error;
}


// Handle movement by checking each axis
void StepperControl::handleMovementInterrupt(void){

        axis1.checkTiming();
        axis2.checkTiming();
        axis3.checkTiming();
}

bool interruptBusy = false;
void handleMovementInterruptTest(void) {
   if (interruptBusy == false) {
      interruptBusy = true;
      StepperControl::getInstance()->handleMovementInterrupt();
      //blinkLed();
      interruptBusy = false;
   }
}

// Start the interrupt used for moviing
// Interrupt management code library written by Paul Stoffregen
void StepperControl::initInterrupt() {
        //Timer1.attachInterrupt(StepperControl::getInstance()->handleMovementInterrupt);
        Timer1.attachInterrupt(handleMovementInterruptTest);
        Timer1.initialize(MOVEMENT_INTERRUPT_SPEED);
        Timer1.stop();
}




unsigned long StepperControl::getMaxLength(unsigned long lengths[3]) {
   unsigned long max = lengths[0];
   for (int i = 1; i < 3; i++) {
      if (lengths[i] > max) {
         max = lengths[i];
      }
   }
   return max;
}

void StepperControl::enableMotors() {
   axis1.enableMotor();
   axis2.enableMotor();
   axis3.enableMotor();
   delay(100);
}

void StepperControl::disableMotors() {
   axis1.disableMotor();
   axis2.disableMotor();
   // axis3.disableMotor(); locks the Z-Axis when stopped
   delay(100);
}

bool StepperControl::endStopsReached() {

   if (axis1.endStopsReached() ||
      axis2.endStopsReached() ||
      axis3.endStopsReached()) {
      return true;
   }
   return false;

}

void StepperControl::reportEndStops() {
   CurrentState::getInstance()->printEndStops();
}

void StepperControl::reportPosition(){
   CurrentState::getInstance()->printPosition();
}

void StepperControl::storeEndStops() {
   CurrentState::getInstance()->storeEndStops();
}

/**
 * water is dosed by setting the pin for the water high for a number of miliseconds
 *
 */

//void StepperControl::doseWaterByTime(long time) {
// digitalWrite(HEATER_1_PIN, HIGH);
// delay(time);
// digitalWrite(HEATER_1_PIN, LOW);
//}


void StepperControl::loadMotorSettings() {

   // Load settings

   homeIsUp[0]    = ParameterList::getInstance()->getValue(MOVEMENT_HOME_UP_X);
   homeIsUp[1]    = ParameterList::getInstance()->getValue(MOVEMENT_HOME_UP_Y);
   homeIsUp[2]    = ParameterList::getInstance()->getValue(MOVEMENT_HOME_UP_Z);

   speedMax[0]    = ParameterList::getInstance()->getValue(MOVEMENT_MAX_SPD_X);
   speedMax[1]    = ParameterList::getInstance()->getValue(MOVEMENT_MAX_SPD_Y);
   speedMax[2]    = ParameterList::getInstance()->getValue(MOVEMENT_MAX_SPD_Z);

   speedMin[0]       = ParameterList::getInstance()->getValue(MOVEMENT_MIN_SPD_X);
   speedMin[1]    = ParameterList::getInstance()->getValue(MOVEMENT_MIN_SPD_Y);
   speedMin[2]    = ParameterList::getInstance()->getValue(MOVEMENT_MIN_SPD_Z);

   stepsAcc[0]       = ParameterList::getInstance()->getValue(MOVEMENT_STEPS_ACC_DEC_X);
   stepsAcc[1]    = ParameterList::getInstance()->getValue(MOVEMENT_STEPS_ACC_DEC_Y);
   stepsAcc[2]    = ParameterList::getInstance()->getValue(MOVEMENT_STEPS_ACC_DEC_Z);

   motorInv[0]       = ParameterList::getInstance()->getValue(MOVEMENT_INVERT_MOTOR_X);
   motorInv[1]    = ParameterList::getInstance()->getValue(MOVEMENT_INVERT_MOTOR_Y);
   motorInv[2]    = ParameterList::getInstance()->getValue(MOVEMENT_INVERT_MOTOR_Z);

   endStInv[0]       = ParameterList::getInstance()->getValue(MOVEMENT_INVERT_ENDPOINTS_X);
   endStInv[1]    = ParameterList::getInstance()->getValue(MOVEMENT_INVERT_ENDPOINTS_Y);
   endStInv[2]    = ParameterList::getInstance()->getValue(MOVEMENT_INVERT_ENDPOINTS_Z);

   timeOut[0]     = ParameterList::getInstance()->getValue(MOVEMENT_TIMEOUT_X);
   timeOut[1]     = ParameterList::getInstance()->getValue(MOVEMENT_TIMEOUT_X);
   timeOut[2]     = ParameterList::getInstance()->getValue(MOVEMENT_TIMEOUT_X);

   axis1.loadMotorSettings(speedMax[0], speedMin[0], stepsAcc[0], timeOut[0], homeIsUp[0], motorInv[0], endStInv[0], MOVEMENT_INTERRUPT_SPEED);
   axis2.loadMotorSettings(speedMax[1], speedMin[1], stepsAcc[1], timeOut[1], homeIsUp[1], motorInv[1], endStInv[1], MOVEMENT_INTERRUPT_SPEED);
   axis3.loadMotorSettings(speedMax[2], speedMin[2], stepsAcc[2], timeOut[2], homeIsUp[2], motorInv[2], endStInv[2], MOVEMENT_INTERRUPT_SPEED);

}
