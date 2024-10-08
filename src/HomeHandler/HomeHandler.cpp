#include "HomeHandler.h"


// Singleton instance getter
HomeHandler& HomeHandler::getInstance() {
    static HomeHandler instance; // Static instance ensures it's created once
    return instance;
}

// Private constructor for singleton
HomeHandler::HomeHandler() {}

void HomeHandler::begin() {

}


// FreeRTOS task function
void HomeHandler::taskWrapper(void* pvParameters) {
    HomeHandler* instance = static_cast<HomeHandler*>(pvParameters);
    instance->taskHome();
}

// Task process function
void HomeHandler::taskHome() {
    while(true) {
        handleHome();
        vTaskDelay(1 / portTICK_PERIOD_MS); // 10ms delay
    }
}

void HomeHandler::handleHome() {
    
    SendHomePhaseA();
}


void HomeHandler::SendHomePhaseA() {
    
    if (!isRunning()){ //flag used to reset and not run constantly
        return;
    }

    WheelHandler& wheelHandler = WheelHandler::getInstance();
    EncoderHandler& encoderHandler = EncoderHandler::getInstance();
    wheelHandler.setMovement(200);
    wheelHandler.setWheelDirection(true); // true CCW, false CW

    if (encoderHandler.objectDetected) {
        Serial.println("[D]: Home found");
        wheelHandler.setWheelDirection(false); // true CCW, false CW
        //vTaskDelay(400 / portTICK_PERIOD_MS); // 10ms delay
        wheelHandler.setMovement(0);
        wheelHandler.setMotorEnabled(false);
        encoderHandler.resetPosition();
        setRun(false);
        return;
    }

}



void HomeHandler::setRun(bool value) {
    running = value;
}

bool HomeHandler::isRunning() {
    return running;
}