// Wheel.h
#ifndef WHEEL_HANDLER_H
#define WHEEL_HANDLER_H

#include <Arduino.h>

class WheelHandler {
public:
    // Singleton instance getter
    static WheelHandler& getInstance();

    // Function to set up wheel
    void begin(int EN, int DA, int DB);

    // Function to process wheel operation
    void handleWheel();

    // Setters and getters for motorEnabled
    void setMotorEnabled(bool enabled);
    bool isMotorEnabled() const;

    // Setters and getters for wheelDirection
    void setWheelDirection(bool direction);
    bool getWheelDirection() const;

    // Setters and getters for movement
    void setMovement(int mov);
    int getMovement() const;

    static void taskWrapper(void* pvParameters);  // Task wrapper function
    void taskWheel();


private:
    // Private constructor for singleton
    WheelHandler();

    // Pin variables
    int pinEN, pinDA, pinDB;

    // Local flags
    bool motorEnabled;
    bool wheelDirection;
    int movement;
};

#endif // WHEEL_H
