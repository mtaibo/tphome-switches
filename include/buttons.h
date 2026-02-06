#ifndef BUTTONS_H
#define BUTTONS_H

class Button {

    private:
        int _pin;
        unsigned long _startTime;
        bool _wasPressed;
        bool _waitingRelease;

        unsigned long get_duration(); 
    
    public:
        Button(int pin);
        void setup();
        void check();        
};

extern Button buttonTop;
extern Button buttonMid;
extern Button buttonBottom;

#endif