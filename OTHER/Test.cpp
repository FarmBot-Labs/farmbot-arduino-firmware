#include <stdio.h>

#include "CurrentState.h"

#define TOLERANCE 3

void setup() {
   Serial.begin(115200);

   long *polar = new long[2]; // (r, t) in steps
   long *rect = new long[2]; // (x, y) in steps

   Serial.print("TOLERANCE = ");
   Serial.println(TOLERANCE);
   Serial.println();

   Serial.println("====  POLAR -> RECT tests  ====\n");

   Serial.print("test 1: polar(0, 0) -> rect(0, 0)");
   polar[0] = 0;
   polar[1] = 0 * STEPS_PER_DEG;
   rect = CurrentState::polarToRect(polar);
   Serial.print(",  result: (");
   Serial.print(rect[0] - RADUIS_STEPS);
   Serial.print(", ");
   Serial.print(rect[1] - RADUIS_STEPS);
   Serial.println(
         rect[0] - RADUIS_STEPS >= 0 - TOLERANCE &&
         rect[0] - RADUIS_STEPS <= 0 + TOLERANCE &&
         rect[1] - RADUIS_STEPS >= 0 - TOLERANCE &&
         rect[1] - RADUIS_STEPS <= 0 + TOLERANCE ?
         ")  [PASS]" : ")  [FAIL]");

   Serial.print("test 2: polar(4242, 0) -> rect(4242, 0)");
   polar[0] = 4242;
   polar[1] = 0 * STEPS_PER_DEG;
   rect = CurrentState::polarToRect(polar);
   Serial.print(",  result: (");
   Serial.print(rect[0] - RADUIS_STEPS);
   Serial.print(", ");
   Serial.print(rect[1] - RADUIS_STEPS);
   Serial.println(
         rect[0] - RADUIS_STEPS >= 4242 - TOLERANCE &&
         rect[0] - RADUIS_STEPS <= 4242 + TOLERANCE &&
         rect[1] - RADUIS_STEPS >= 0 - TOLERANCE &&
         rect[1] - RADUIS_STEPS <= 0 + TOLERANCE ?
         ")  [PASS]" : ")  [FAIL]");

   Serial.print("test 3: polar(4242, 180) -> rect(-4242, 0)");
   polar[0] = 4242;
   polar[1] = 180 * STEPS_PER_DEG;
   rect = CurrentState::polarToRect(polar);
   Serial.print(",  result: (");
   Serial.print(rect[0] - RADUIS_STEPS);
   Serial.print(", ");
   Serial.print(rect[1] - RADUIS_STEPS);
   Serial.println(
         rect[0] - RADUIS_STEPS >= -4242 - TOLERANCE &&
         rect[0] - RADUIS_STEPS <= -4242 + TOLERANCE &&
         rect[1] - RADUIS_STEPS >= 0 - TOLERANCE &&
         rect[1] - RADUIS_STEPS <= 0 + TOLERANCE ?
         ")  [PASS]" : ")  [FAIL]");

   Serial.print("test 4: polar(4242, 270) -> rect(0, -4242)");
   polar[0] = 4242;
   polar[1] = 270 * STEPS_PER_DEG;
   rect = CurrentState::polarToRect(polar);
   Serial.print(",  result: (");
   Serial.print(rect[0] - RADUIS_STEPS);
   Serial.print(", ");
   Serial.print(rect[1] - RADUIS_STEPS);
   Serial.println(
         rect[0] - RADUIS_STEPS >= 0 - TOLERANCE &&
         rect[0] - RADUIS_STEPS <= 0 + TOLERANCE &&
         rect[1] - RADUIS_STEPS >= -4242 - TOLERANCE &&
         rect[1] - RADUIS_STEPS <= -4242 + TOLERANCE ?
         ")  [PASS]" : ")  [FAIL]");

   Serial.print("test 5: polar(4242, 90) -> rect(0, 4242)");
   polar[0] = 4242;
   polar[1] = 90 * STEPS_PER_DEG;
   rect = CurrentState::polarToRect(polar);
   Serial.print(",  result: (");
   Serial.print(rect[0] - RADUIS_STEPS);
   Serial.print(", ");
   Serial.print(rect[1] - RADUIS_STEPS);
   Serial.println(
         rect[0] - RADUIS_STEPS >= 0 - TOLERANCE &&
         rect[0] - RADUIS_STEPS <= 0 + TOLERANCE &&
         rect[1] - RADUIS_STEPS >= 4242 - TOLERANCE &&
         rect[1] - RADUIS_STEPS <= 4242 + TOLERANCE ?
         ")  [PASS]" : ")  [FAIL]");

   Serial.print("test 6: polar(6000, 0) -> rect(6000, 0)");
   polar[0] = 6000;
   polar[1] = 0 * STEPS_PER_DEG;
   rect = CurrentState::polarToRect(polar);
   Serial.print(",  result: (");
   Serial.print(rect[0] - RADUIS_STEPS);
   Serial.print(", ");
   Serial.print(rect[1] - RADUIS_STEPS);
   Serial.println(
         rect[0] - RADUIS_STEPS >= 6000 - TOLERANCE &&
         rect[0] - RADUIS_STEPS <= 6000 + TOLERANCE &&
         rect[1] - RADUIS_STEPS >= 0 - TOLERANCE &&
         rect[1] - RADUIS_STEPS <= 0 + TOLERANCE ?
         ")  [PASS]" : ")  [FAIL]");

   Serial.print("test 7: polar(6000, 135) -> rect(-4242, 4242)");
   polar[0] = 6000;
   polar[1] = 135 * STEPS_PER_DEG;
   rect = CurrentState::polarToRect(polar);
   Serial.print(",  result: (");
   Serial.print(rect[0] - RADUIS_STEPS);
   Serial.print(", ");
   Serial.print(rect[1] - RADUIS_STEPS);
   Serial.println(
         rect[0] - RADUIS_STEPS >= -4242 - TOLERANCE &&
         rect[0] - RADUIS_STEPS <= -4242 + TOLERANCE &&
         rect[1] - RADUIS_STEPS >= 4242 - TOLERANCE &&
         rect[1] - RADUIS_STEPS <= 4242 + TOLERANCE ?
         ")  [PASS]" : ")  [FAIL]");

   Serial.print("test 8: polar(6000, 180) -> rect(-6000, 0)");
   polar[0] = 6000;
   polar[1] = 180 * STEPS_PER_DEG;
   rect = CurrentState::polarToRect(polar);
   Serial.print(",  result: (");
   Serial.print(rect[0] - RADUIS_STEPS);
   Serial.print(", ");
   Serial.print(rect[1] - RADUIS_STEPS);
   Serial.println(
         rect[0] - RADUIS_STEPS >= -6000 - TOLERANCE &&
         rect[0] - RADUIS_STEPS <= -6000 + TOLERANCE &&
         rect[1] - RADUIS_STEPS >= 0 - TOLERANCE &&
         rect[1] - RADUIS_STEPS <= 0 + TOLERANCE ?
         ")  [PASS]" : ")  [FAIL]");

   Serial.print("test 9: polar(6000, 225) -> rect(-4242, -4242)");
   polar[0] = 6000;
   polar[1] = 225 * STEPS_PER_DEG;
   rect = CurrentState::polarToRect(polar);
   Serial.print(",  result: (");
   Serial.print(rect[0] - RADUIS_STEPS);
   Serial.print(", ");
   Serial.print(rect[1] - RADUIS_STEPS);
   Serial.println(
         rect[0] - RADUIS_STEPS >= -4242 - TOLERANCE &&
         rect[0] - RADUIS_STEPS <= -4242 + TOLERANCE &&
         rect[1] - RADUIS_STEPS >= -4242 - TOLERANCE &&
         rect[1] - RADUIS_STEPS <= -4242 + TOLERANCE ?
         ")  [PASS]" : ")  [FAIL]");

   Serial.print("test 10: polar(6000, 270) -> rect(0, -6000)");
   polar[0] = 6000;
   polar[1] = 270 * STEPS_PER_DEG;
   rect = CurrentState::polarToRect(polar);
   Serial.print(",  result: (");
   Serial.print(rect[0] - RADUIS_STEPS);
   Serial.print(", ");
   Serial.print(rect[1] - RADUIS_STEPS);
   Serial.println(
         rect[0] - RADUIS_STEPS >= 0 - TOLERANCE &&
         rect[0] - RADUIS_STEPS <= 0 + TOLERANCE &&
         rect[1] - RADUIS_STEPS >= -6000 - TOLERANCE &&
         rect[1] - RADUIS_STEPS <= -6000 + TOLERANCE ?
         ")  [PASS]" : ")  [FAIL]");

   Serial.print("test 11: polar(6000, 315) -> rect(4242, -4242)");
   polar[0] = 6000;
   polar[1] = 315 * STEPS_PER_DEG;
   rect = CurrentState::polarToRect(polar);
   Serial.print(",  result: (");
   Serial.print(rect[0] - RADUIS_STEPS);
   Serial.print(", ");
   Serial.print(rect[1] - RADUIS_STEPS);
   Serial.println(
         rect[0] - RADUIS_STEPS >= 4242 - TOLERANCE &&
         rect[0] - RADUIS_STEPS <= 4242 + TOLERANCE &&
         rect[1] - RADUIS_STEPS >= -4242 - TOLERANCE &&
         rect[1] - RADUIS_STEPS <= -4242 + TOLERANCE ?
         ")  [PASS]" : ")  [FAIL]");

   Serial.print("test 12: polar(6000, 45) -> rect(4242, 4242)");
   polar[0] = 6000;
   polar[1] = 45 * STEPS_PER_DEG;
   rect = CurrentState::polarToRect(polar);
   Serial.print(",  result: (");
   Serial.print(rect[0] - RADUIS_STEPS);
   Serial.print(", ");
   Serial.print(rect[1] - RADUIS_STEPS);
   Serial.println(
         rect[0] - RADUIS_STEPS >= 4242 - TOLERANCE &&
         rect[0] - RADUIS_STEPS <= 4242 + TOLERANCE &&
         rect[1] - RADUIS_STEPS >= 4242 - TOLERANCE &&
         rect[1] - RADUIS_STEPS <= 4242 + TOLERANCE ?
         ")  [PASS]" : ")  [FAIL]");

   Serial.print("test 13: polar(6000, 90) -> rect(0, 6000)");
   polar[0] = 6000;
   polar[1] = 90 * STEPS_PER_DEG;
   rect = CurrentState::polarToRect(polar);
   Serial.print(",  result: (");
   Serial.print(rect[0] - RADUIS_STEPS);
   Serial.print(", ");
   Serial.print(rect[1] - RADUIS_STEPS);
   Serial.println(
         rect[0] - RADUIS_STEPS >= 0 - TOLERANCE &&
         rect[0] - RADUIS_STEPS <= 0 + TOLERANCE &&
         rect[1] - RADUIS_STEPS >= 6000 - TOLERANCE &&
         rect[1] - RADUIS_STEPS <= 6000 + TOLERANCE ?
         ")  [PASS]" : ")  [FAIL]");

   Serial.println("\n====  RECT -> POLAR tests  ====\n");

   Serial.print("test 1: rect(0, 0) -> polar(0, 0)");
   rect[0] = 0 + RADUIS_STEPS;
   rect[1] = 0 + RADUIS_STEPS;
   polar = CurrentState::rectToPolar(rect);
   Serial.print(",  result: (");
   Serial.print(polar[0]);
   Serial.print(", ");
   Serial.print((long) (polar[1] / STEPS_PER_DEG));
   Serial.println(
         polar[0] >= 0 - TOLERANCE &&
         polar[0] <= 0 + TOLERANCE &&
         (long) (polar[1] / STEPS_PER_DEG) >= 0 - TOLERANCE &&
         (long) (polar[1] / STEPS_PER_DEG) <= 0 + TOLERANCE ?
         ")  [PASS]" : ")  [FAIL]");

   Serial.print("test 2: rect(4242, 0) -> polar(4242, 0)");
   rect[0] = 4242 + RADUIS_STEPS;
   rect[1] = 0 + RADUIS_STEPS;
   polar = CurrentState::rectToPolar(rect);
   Serial.print(",  result: (");
   Serial.print(polar[0]);
   Serial.print(", ");
   Serial.print((long) (polar[1] / STEPS_PER_DEG));
   Serial.println(
         polar[0] >= 4242 - TOLERANCE &&
         polar[0] <= 4242 + TOLERANCE &&
         (long) (polar[1] / STEPS_PER_DEG) >= 0 - TOLERANCE &&
         (long) (polar[1] / STEPS_PER_DEG) <= 0 + TOLERANCE ?
         ")  [PASS]" : ")  [FAIL]");

   Serial.print("test 3: rect(-4242, 0) -> polar(4242, 180)");
   rect[0] = -4242 + RADUIS_STEPS;
   rect[1] = 0 + RADUIS_STEPS;
   polar = CurrentState::rectToPolar(rect);
   Serial.print(",  result: (");
   Serial.print(polar[0]);
   Serial.print(", ");
   Serial.print((long) (polar[1] / STEPS_PER_DEG));
   Serial.println(
         polar[0] >= 4242 - TOLERANCE &&
         polar[0] <= 4242 + TOLERANCE &&
         (long) (polar[1] / STEPS_PER_DEG) >= 180 - TOLERANCE &&
         (long) (polar[1] / STEPS_PER_DEG) <= 180 + TOLERANCE ?
         ")  [PASS]" : ")  [FAIL]");

   Serial.print("test 4: rect(0, -4242) -> polar(4242, 270)");
   rect[0] = 0 + RADUIS_STEPS;
   rect[1] = -4242 + RADUIS_STEPS;
   polar = CurrentState::rectToPolar(rect);
   Serial.print(",  result: (");
   Serial.print(polar[0]);
   Serial.print(", ");
   Serial.print((long) (polar[1] / STEPS_PER_DEG));
   Serial.println(
         polar[0] >= 4242 - TOLERANCE &&
         polar[0] <= 4242 + TOLERANCE &&
         (long) (polar[1] / STEPS_PER_DEG) >= 270 - TOLERANCE &&
         (long) (polar[1] / STEPS_PER_DEG) <= 270 + TOLERANCE ?
         ")  [PASS]" : ")  [FAIL]");

   Serial.print("test 5: rect(0, 4242) -> polar(4242, 90)");
   rect[0] = 0 + RADUIS_STEPS;
   rect[1] = 4242 + RADUIS_STEPS;
   polar = CurrentState::rectToPolar(rect);
   Serial.print(",  result: (");
   Serial.print(polar[0]);
   Serial.print(", ");
   Serial.print((long) (polar[1] / STEPS_PER_DEG));
   Serial.println(
         polar[0] >= 4242 - TOLERANCE &&
         polar[0] <= 4242 + TOLERANCE &&
         (long) (polar[1] / STEPS_PER_DEG) >= 90 - TOLERANCE &&
         (long) (polar[1] / STEPS_PER_DEG) <= 90 + TOLERANCE ?
         ")  [PASS]" : ")  [FAIL]");

   Serial.print("test 6: rect(6000, 0) -> polar(6000, 0)");
   rect[0] = 6000 + RADUIS_STEPS;
   rect[1] = 0 + RADUIS_STEPS;
   polar = CurrentState::rectToPolar(rect);
   Serial.print(",  result: (");
   Serial.print(polar[0]);
   Serial.print(", ");
   Serial.print((long) (polar[1] / STEPS_PER_DEG));
   Serial.println(
         polar[0] >= 6000 - TOLERANCE &&
         polar[0] <= 6000 + TOLERANCE &&
         (long) (polar[1] / STEPS_PER_DEG) >= 0 - TOLERANCE &&
         (long) (polar[1] / STEPS_PER_DEG) <= 0 + TOLERANCE ?
         ")  [PASS]" : ")  [FAIL]");

   Serial.print("test 7: rect(-4242, 4242) -> polar(6000, 135)");
   rect[0] = -4242 + RADUIS_STEPS;
   rect[1] = 4242 + RADUIS_STEPS;
   polar = CurrentState::rectToPolar(rect);
   Serial.print(",  result: (");
   Serial.print(polar[0]);
   Serial.print(", ");
   Serial.print((long) (polar[1] / STEPS_PER_DEG));
   Serial.println(
         polar[0] >= 6000 - TOLERANCE &&
         polar[0] <= 6000 + TOLERANCE &&
         (long) (polar[1] / STEPS_PER_DEG) >= 135 - TOLERANCE &&
         (long) (polar[1] / STEPS_PER_DEG) <= 135 + TOLERANCE ?
         ")  [PASS]" : ")  [FAIL]");

   Serial.print("test 8: rect(-6000, 0) -> polar(6000, 180)");
   rect[0] = -6000 + RADUIS_STEPS;
   rect[1] = 0 + RADUIS_STEPS;
   polar = CurrentState::rectToPolar(rect);
   Serial.print(",  result: (");
   Serial.print(polar[0]);
   Serial.print(", ");
   Serial.print((long) (polar[1] / STEPS_PER_DEG));
   Serial.println(
         polar[0] >= 6000 - TOLERANCE &&
         polar[0] <= 6000 + TOLERANCE &&
         (long) (polar[1] / STEPS_PER_DEG) >= 180 - TOLERANCE &&
         (long) (polar[1] / STEPS_PER_DEG) <= 180 + TOLERANCE ?
         ")  [PASS]" : ")  [FAIL]");

   Serial.print("test 9: rect(-4242, -4242) -> polar(6000, 225)");
   rect[0] = -4242 + RADUIS_STEPS;
   rect[1] = -4242 + RADUIS_STEPS;
   polar = CurrentState::rectToPolar(rect);
   Serial.print(",  result: (");
   Serial.print(polar[0]);
   Serial.print(", ");
   Serial.print((long) (polar[1] / STEPS_PER_DEG));
   Serial.println(
         polar[0] >= 6000 - TOLERANCE &&
         polar[0] <= 6000 + TOLERANCE &&
         (long) (polar[1] / STEPS_PER_DEG) >= 225 - TOLERANCE &&
         (long) (polar[1] / STEPS_PER_DEG) <= 225 + TOLERANCE ?
         ")  [PASS]" : ")  [FAIL]");

   Serial.print("test 10: rect(0, -6000) -> polar(6000, 270)");
   rect[0] = 0 + RADUIS_STEPS;
   rect[1] = -6000 + RADUIS_STEPS;
   polar = CurrentState::rectToPolar(rect);
   Serial.print(",  result: (");
   Serial.print(polar[0]);
   Serial.print(", ");
   Serial.print((long) (polar[1] / STEPS_PER_DEG));
   Serial.println(
         polar[0] >= 6000 - TOLERANCE &&
         polar[0] <= 6000 + TOLERANCE &&
         (long) (polar[1] / STEPS_PER_DEG) >= 270 - TOLERANCE &&
         (long) (polar[1] / STEPS_PER_DEG) <= 270 + TOLERANCE ?
         ")  [PASS]" : ")  [FAIL]");

   Serial.print("test 11: rect(4242, -4242) -> polar(6000, 315)");
   rect[0] = 4242 + RADUIS_STEPS;
   rect[1] = -4242 + RADUIS_STEPS;
   polar = CurrentState::rectToPolar(rect);
   Serial.print(",  result: (");
   Serial.print(polar[0]);
   Serial.print(", ");
   Serial.print((long) (polar[1] / STEPS_PER_DEG));
   Serial.println(
         polar[0] >= 6000 - TOLERANCE &&
         polar[0] <= 6000 + TOLERANCE &&
         (long) (polar[1] / STEPS_PER_DEG) >= 315 - TOLERANCE &&
         (long) (polar[1] / STEPS_PER_DEG) <= 315 + TOLERANCE ?
         ")  [PASS]" : ")  [FAIL]");

   Serial.print("test 12: rect(4242, 4242) -> polar(6000, 45)");
   rect[0] = 4242 + RADUIS_STEPS;
   rect[1] = 4242 + RADUIS_STEPS;
   polar = CurrentState::rectToPolar(rect);
   Serial.print(",  result: (");
   Serial.print(polar[0]);
   Serial.print(", ");
   Serial.print((long) (polar[1] / STEPS_PER_DEG));
   Serial.println(
         polar[0] >= 6000 - TOLERANCE &&
         polar[0] <= 6000 + TOLERANCE &&
         (long) (polar[1] / STEPS_PER_DEG) >= 45 - TOLERANCE &&
         (long) (polar[1] / STEPS_PER_DEG) <= 45 + TOLERANCE ?
         ")  [PASS]" : ")  [FAIL]");

   Serial.print("test 13: rect(0, 6000) -> polar(6000, 90)");
   rect[0] = 0 + RADUIS_STEPS;
   rect[1] = 6000 + RADUIS_STEPS;
   polar = CurrentState::rectToPolar(rect);
   Serial.print(",  result: (");
   Serial.print(polar[0]);
   Serial.print(", ");
   Serial.print((long) (polar[1] / STEPS_PER_DEG));
   Serial.println(
         polar[0] >= 6000 - TOLERANCE &&
         polar[0] <= 6000 + TOLERANCE &&
         (long) (polar[1] / STEPS_PER_DEG) >= 90 - TOLERANCE &&
         (long) (polar[1] / STEPS_PER_DEG) <= 90 + TOLERANCE ?
         ")  [PASS]" : ")  [FAIL]");
}

void loop() {}
