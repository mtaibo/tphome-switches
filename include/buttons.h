#ifndef BUTTONS
#define BUTTONS

class Button {

    private:
        int _pin;
        unsigned long _startTime;
        bool _isPressed;

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