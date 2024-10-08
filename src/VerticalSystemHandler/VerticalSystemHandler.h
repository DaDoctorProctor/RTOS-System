// VerticalSystemHandler.h
#ifndef VERTICAL_SYSTEM_HANDLER_H
#define VERTICAL_SYSTEM_HANDLER_H

#include <Arduino.h>

class VerticalSystemHandler {
public:
    // Function to process vertical system operation
    static VerticalSystemHandler& getInstance();
    void begin(int DIR, int STEP, int ENABLE, int LOWER_LIMIT, int UPPER_LIMIT);
    void handleVerticalSystem();
    static void taskWrapper(void* pvParameters); 
    void taskVerticalSystem();

    // Setters and getters for motorEnabled
    void setVerticalSystemEnabled(bool enabled);
    bool isVerticalSystemEnabled() const;

    // Setters and getters for motorEnabled
    void setSteps(int stp);
    int getSteps() const;

    void setStepsMoved(int stp);
    int getStepsMoved() const;

    void activateStop();

    void Calibrate();

    void setPixelsY(int py);
    int getPixelsY() const;

     int VerticalInterpreter(int pixels_y);

private:
    // Private constructor for singleton
    VerticalSystemHandler();

    int DIR_PIN;
    int STEP_PIN;
    int ENABLE_PIN;
    int LOWER_LIMIT_PIN;
    int UPPER_LIMIT_PIN;

    void EvaluateDirection(int inputValue);

    void VerticalUp(int steps);
    void VerticalDown(int steps);

    void FinishTask();

    bool LimitSwitchLow();
    bool LimitSwitchUp();

    int steps;

    int stepsMoved;

    bool verticalSystemEnabled;

    int pixelsY;

};

#endif // VERTICAL_SYSTEM_HANDLER_H
