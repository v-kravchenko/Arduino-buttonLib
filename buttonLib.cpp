#include "buttonlib.h"

buttonLib::buttonLib(uint8_t pin, callbackFunction clickFunction) {
    this->buttonPin = pin;
    this->clickFunc = clickFunction;
    this->buttonPressed = -1;
    //this->repeat1 = 0;
    //this->repeat2 = 0;
}

/*void buttonLib::setRepeat(unsigned int repeat1, unsigned int repeat2) {
    this->repeat1 = repeat1;
    this->repeat2 = repeat2;    
}*/

void buttonLib::update() {
    
    unsigned long currentMillis = millis();
    static unsigned long millisRepeat = 0;
    static bool repeatMode = 0;

    if (buttonPressed == -1) {
        for (uint8_t i = 0; i < BUTTON_COUNT; i++ ) {
            if (debounce(buttonPressed == i, i)) {
                buttonPressed = i;
                millisRepeat = currentMillis;
                break;
            }
        }        
    } else {
        if (debounce(true, buttonPressed)) {

            if (currentMillis - millisRepeat > (repeatMode == 0 ? 1000 : 200)) {
                repeatMode = 1;
                millisRepeat = currentMillis;
                if (1000 > 0 && clickFunc) clickFunc(buttonPressed);      
            }
            
        } else {
            repeatMode = 0;
            if (clickFunc) clickFunc(buttonPressed);
            buttonPressed = -1;
        }
    }
}
    
bool buttonLib::readButton(uint8_t buttonNum) {
    int sensorValue = analogRead(buttonPin);

	#if BUTTON_DEBUG
		if (sensorValue > 0) {
			Serial.print(sensorValue * 5.0f / 1023.0f);
			Serial.print("V, ");
			Serial.print(sensorValue);
			Serial.println();
		}
	#endif

	#if BUTTON_COUNT == 3
		if (buttonNum == 0 && sensorValue > 767)  return true;
		else if (buttonNum == 1 && sensorValue >= 374 && sensorValue <= 767)  return true;
		else if (buttonNum == 2 && sensorValue > 187 && sensorValue < 374)  return true;
	#elif BUTTON_COUNT == 4
		if (sensorValue > 895 && buttonNum == 0) return true;
		if (sensorValue > 639 && sensorValue < 895 && buttonNum == 1) return true;
		if (sensorValue > 383 && sensorValue < 639 && buttonNum == 2) return true;
		if (sensorValue > 127 && sensorValue < 383 && buttonNum == 3) return true;	
	#elif BUTTON_COUNT == 6
		if (sensorValue >= 940 && buttonNum == 0) return true;
		if (sensorValue >= 770 && sensorValue < 940  && buttonNum == 1) return true;
		if (sensorValue >= 600 && sensorValue < 770  && buttonNum == 2) return true;
		if (sensorValue >= 430 && sensorValue < 600  && buttonNum == 3) return true;
		if (sensorValue >= 255 && sensorValue < 430  && buttonNum == 4) return true;
		if (sensorValue >= 80  && sensorValue < 255  && buttonNum == 5) return true;
	#endif

	return false;
}

// Процедура определения нажатия кнопки без дребезга:
bool buttonLib::debounce(bool last, uint8_t buttonNum) {
    bool current = readButton(buttonNum); // считываем текущее состояние кнопки
    if (last != current) { // если состояние изменилось
        delay(5); // делаем задержку на 5 мсек, пока уляжется дребезг
        current = readButton(buttonNum); // и считываем снова
    }
    return current; // возвращаем текущее состояние кнопки
}
