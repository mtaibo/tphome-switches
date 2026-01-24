#ifndef ACTIONS_H
#define ACTIONS_H

#include <Arduino.h>

void handleButtonAction(int pin, unsigned long duration);
void updateActions();

void startBlink(int led);

void blindUp();
void blindDown();
void blindStop();

#endif