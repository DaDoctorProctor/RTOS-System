#ifndef FUZZYHANDLER_H
#define FUZZYHANDLER_H

#include <Arduino.h>
#include <Fuzzy.h>
#include "CommandHandler/ModeSelector.h"
#include "Wheel/EncoderHandler.h"
#include "Wheel/WheelHandler.h"


class FuzzyHandler {
public:
    // Constructor
    FuzzyHandler();

    // Public methods
    void begin();

    // Getter and Setter for reference
    int getReference() const;
    void setReference(int newRef);

    void handleFuzzy(); 
    static void taskWrapper(void* pvParameters);  // Task wrapper function
    static FuzzyHandler& getInstance(); // Singleton instance getter

    void setPixelsX(int px);
    int getPixelsX() const;

    int HorizontalInterpreter(int pixels_x);

private:
    // Private variables
    int ref;
    volatile int pos_act;
    float ekT_ant;
    float sum_ekT;
    Fuzzy *fuzzy;

    // Private methods
    void taskFuzzy();
    void sentidoHor(byte vel);
    void FuzzyControllerProcess();
    void sentidoAntiHor(byte vel);
    void freno();

    const int fT = 25;

    int pixelsX;
    
};


#endif // FUZZYHANDLER_H
