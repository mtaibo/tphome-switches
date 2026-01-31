#ifndef ACTIONS
#define ACTIONS

#include <Arduino.h>

enum Direction { UP = 1, DOWN = 0, STOP = -1 };

void blink(int pin, int control);

void handleButtonAction(int pin, unsigned long duration);
void moveBlind(Direction direction);
void updateActions();

#endif