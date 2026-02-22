#ifndef ACTIONS
#define ACTIONS

enum Direction { UP = 1, DOWN = 0, STOP = -1 };

void blink(int pin, int control);
void set_position(float next_position);

void handle_button_action(int pin, unsigned long duration);
void move_blind(Direction direction);
void update_actions();

#endif
