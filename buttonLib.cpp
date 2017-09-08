#include "buttonlib.h"

buttonLib::buttonLib(int pin, callbackFunction clickFunction) {
    this->buttonPin = pin;
    this->clickFunc = clickFunction;
    this->buttonPressed = -1;
    this->buttonCount = 3;
    this->repeat1 = 0;
    this->repeat2 = 0;
}

void buttonLib::setRepeat(unsigned int repeat1, unsigned int repeat2) {
    this->repeat1 = repeat1;
    this->repeat2 = repeat2;    
}

void buttonLib::update() {
    
    unsigned long currentMillis = millis();
    static unsigned long millisRepeat = 0;
    static bool repeatMode = 0;

    if (buttonPressed == -1) {
        for (byte i = 0; i < buttonCount; i++ ) {
            if (debounce(buttonPressed == i, i)) {
                buttonPressed = i;
                millisRepeat = currentMillis;
                break;
            }
        }        
    } else {
        if (debounce(true, buttonPressed)) {

            if (currentMillis - millisRepeat > (repeatMode == 0 ? this->repeat1 : this->repeat2)) {
                repeatMode = 1;
                millisRepeat = currentMillis;
                if (this->repeat1 > 0 && clickFunc) clickFunc(buttonPressed);      
            }
            
        } else {
            repeatMode = 0;
            if (clickFunc) clickFunc(buttonPressed);
            buttonPressed = -1;
        }
    }
}
    
bool buttonLib::readButton(byte buttonNum) {
    int sensorValue = analogRead(buttonPin);

    if (buttonNum == 0 && sensorValue > 767)  return true;
    else if (buttonNum == 1 && sensorValue >= 374 && sensorValue <= 767)  return true;
    else if (buttonNum == 2 && sensorValue > 187 && sensorValue < 374)  return true;
    else return false;
}

// Процедура определения нажатия кнопки без дребезга:
bool buttonLib::debounce(bool last, byte buttonNum) {
    bool current = readButton(buttonNum); // считываем текущее состояние кнопки
    if (last != current) { // если состояние изменилось
        delay(5); // делаем задержку на 5 мсек, пока уляжется дребезг
        current = readButton(buttonNum); // и считываем снова
    }
    return current; // возвращаем текущее состояние кнопки
}
