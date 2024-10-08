// Wheel.cpp
#include "WheelHandler.h"

// Singleton instance getter
WheelHandler& WheelHandler::getInstance() {
    static WheelHandler instance; // Static instance ensures it's created once
    return instance;
}

// Private constructor for singleton
WheelHandler::WheelHandler() : motorEnabled(false), wheelDirection(false), movement(0) {}

// FreeRTOS task function
void WheelHandler::taskWrapper(void* pvParameters) {
    WheelHandler* instance = static_cast<WheelHandler*>(pvParameters);
    instance->taskWheel();
}

// Task process function
void WheelHandler::taskWheel() {
    while(true) {
        handleWheel();
        vTaskDelay(10 / portTICK_PERIOD_MS); // 10ms delay
    }
}


// Function to set up wheel
void WheelHandler::begin(int EN, int DA, int DB) {
    pinEN = EN;
    pinDA = DA;
    pinDB = DB;

    pinMode(EN, OUTPUT);
    pinMode(DA, OUTPUT);
    pinMode(DB, OUTPUT);

    Serial.printf("[D] Wheel: ONLINE\n");
}

// Function to process wheel operation
void WheelHandler::handleWheel() {
    if (motorEnabled) {
        if (wheelDirection) {
            digitalWrite(pinDA, LOW);
            digitalWrite(pinDB, HIGH);
        } else {
            digitalWrite(pinDA, HIGH);
            digitalWrite(pinDB, LOW);
        }
        analogWrite(pinEN, movement);
    } else {
        analogWrite(pinEN, 0);
        digitalWrite(pinDA, LOW);
        digitalWrite(pinDB, LOW);
    }
}


// Setter for motorEnabled
void WheelHandler::setMotorEnabled(bool enabled) {
    motorEnabled = enabled;
}

// Getter for motorEnabled
bool WheelHandler::isMotorEnabled() const {
    return motorEnabled;
}

// Setter for wheelDirection
void WheelHandler::setWheelDirection(bool direction) {
    wheelDirection = direction;
}

// Getter for wheelDirection
bool WheelHandler::getWheelDirection() const {
    return wheelDirection;
}

// Setter for movement
void WheelHandler::setMovement(int mov) {
    movement = mov;
}

// Getter for movement
int WheelHandler::getMovement() const {
    return movement;
}
