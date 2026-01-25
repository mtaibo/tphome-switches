#ifndef ACTIONS
#define ACTIONS

#include <Arduino.h>

void handleButtonAction(int pin, unsigned long duration);
void updateActions();

void startBlink(int led);

void blindUp();
void blindDown();
void blindStop();

#endif