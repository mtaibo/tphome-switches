#ifndef ACTIONS
#define ACTIONS

#include <Arduino.h>

// An enum to simplify direction reading and to prevent errors
enum Direction { UP = 1, DOWN = 0, STOP = -1 };

// Secondary actions
void startBlink(int led);

// Movement action
void moveBlind(Direction direction);

// Main actions handlers
void handleButtonAction(int pin, unsigned long duration);
void updateActions();

#endif