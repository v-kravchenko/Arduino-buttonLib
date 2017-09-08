#ifndef buttonLib_h
#define buttonLib_h

#include "Arduino.h"

extern "C" {
    typedef void (*callbackFunction)(byte);
}

class buttonLib 
{
public:
    buttonLib(int pin, callbackFunction clickFunction);
    void update();
    void setRepeat(unsigned int repeat1, unsigned int repeat2);
private:
    bool readButton(byte buttonNum);
    bool debounce(bool last, byte buttonNum);

    int buttonPin;
    int buttonPressed;
    byte buttonCount;
    unsigned long millisRepeat;

    unsigned int repeat1;
    unsigned int repeat2;
    byte repeatMode;     

    callbackFunction clickFunc;  
};

#endif