#ifndef buttonLib_h
#define buttonLib_h

#include "Arduino.h"

#define BUTTON_COUNT 6
#define BUTTON_DEBUG false

extern "C" {
    typedef void (*callbackFunction)(uint8_t);
}

class buttonLib 
{
public:
    buttonLib(uint8_t pin, callbackFunction clickFunction);
    void update();
private:
    bool readButton(uint8_t buttonNum);
    bool debounce(bool last, uint8_t buttonNum);

    uint8_t buttonPin;
    char buttonPressed;
    callbackFunction clickFunc;  
};

#endif
