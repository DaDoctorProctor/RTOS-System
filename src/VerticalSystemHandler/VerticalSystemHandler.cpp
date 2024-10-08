// Wheel.cpp
#include "VerticalSystemHandler.h"

// Singleton instance getter
VerticalSystemHandler& VerticalSystemHandler::getInstance() {
    static VerticalSystemHandler instance; // Static instance ensures it's created once
    return instance;
}

// Private constructor for singleton
VerticalSystemHandler::VerticalSystemHandler() {}

// FreeRTOS task function
void VerticalSystemHandler::taskWrapper(void* pvParameters) {
    VerticalSystemHandler* instance = static_cast<VerticalSystemHandler*>(pvParameters);
    instance->taskVerticalSystem();
}

// Task process function
void VerticalSystemHandler::taskVerticalSystem() {
    while(true) {
        handleVerticalSystem();
        vTaskDelay(10 / portTICK_PERIOD_MS); // 10ms delay
    }
}

void VerticalSystemHandler::begin(int DIR, int STEP, int ENABLE, int LOWER_LIMIT, int UPPER_LIMIT){

    DIR_PIN = DIR;
    STEP_PIN = STEP;
    ENABLE_PIN = ENABLE;
    LOWER_LIMIT_PIN = LOWER_LIMIT;
    UPPER_LIMIT_PIN = UPPER_LIMIT;

    pinMode(DIR_PIN, OUTPUT);
    pinMode(STEP_PIN, OUTPUT);
    pinMode(ENABLE_PIN, OUTPUT);
    pinMode(LOWER_LIMIT_PIN, INPUT); // Assuming using internal pull-up resistors
    pinMode(UPPER_LIMIT_PIN, INPUT);

    // Enable the motor
    digitalWrite(ENABLE_PIN, LOW);  // LOW to enable, HIGH to disable
   
}

void VerticalSystemHandler::handleVerticalSystem() {

    LimitSwitchLow();
    LimitSwitchUp();
    if (!isVerticalSystemEnabled()) {
        return; // Exit the function if verticalSystemEnabled is false
    }

    EvaluateDirection(getSteps());
    
}


void VerticalSystemHandler::activateStop() {
    FinishTask();
}

void VerticalSystemHandler::Calibrate(){
    EvaluateDirection(-1000000);
    EvaluateDirection(1000000);
    EvaluateDirection(stepsMoved/2);
    Serial.println("The system moved" + String(stepsMoved));
}

// MOVEMENT CORE FEATURES
void VerticalSystemHandler::EvaluateDirection(int inputValue){
    if (inputValue != 0) {
        if (inputValue > 0) {
        VerticalUp(inputValue); // Move up by inputValue steps
        } else {
        VerticalDown(abs(inputValue)); // Move down by absolute value of inputValue steps
        }
    }
}

void VerticalSystemHandler::VerticalUp(int steps) {
    if (LimitSwitchUp()){
        FinishTask();
        return;
    }
    Serial.print("Moving Up ");
    Serial.print(steps);
    Serial.println(" steps");
    digitalWrite(DIR_PIN, LOW); // Set direction to forward
    for (int i = 0; i < steps; i++) {
        if (LimitSwitchUp() || !isVerticalSystemEnabled()){
            FinishTask();
            return;
        }
        digitalWrite(STEP_PIN, HIGH);
        delayMicroseconds(10);
        digitalWrite(STEP_PIN, LOW);
        delayMicroseconds(10);
        setStepsMoved(i);
    }
    FinishTask();
    return;
}

void VerticalSystemHandler::VerticalDown(int steps) {
    if (LimitSwitchLow()){
        FinishTask();
        return;
    }
    Serial.print("Moving Down ");
    Serial.print(steps);
    Serial.println(" steps");
    digitalWrite(DIR_PIN, HIGH); // Set direction to reverse
    for (int i = 0; i < steps; i++) {
        if (LimitSwitchLow() || !isVerticalSystemEnabled()){
            FinishTask();
            return;
        }
        digitalWrite(STEP_PIN, HIGH);
        delayMicroseconds(10);
        digitalWrite(STEP_PIN, LOW);
        delayMicroseconds(10);
        setStepsMoved(i);
    }
    FinishTask();
    return;
}

void VerticalSystemHandler::FinishTask(){
    setVerticalSystemEnabled(false);
    Serial.println("[D]: Task complete");
}


bool VerticalSystemHandler::LimitSwitchLow(){
    if (digitalRead(LOWER_LIMIT_PIN)){
        //Serial.println("[D]: LL: Activated");
        return true;
    } else {
        return false;
    }
}

bool VerticalSystemHandler::LimitSwitchUp(){
    if (digitalRead(UPPER_LIMIT_PIN)){
        //Serial.println("[D]: UL: Activated");
        return true;
    } else {
        return false;
    }
}



int VerticalSystemHandler::VerticalInterpreter(int pixels_y){
    if (pixels_y >= -4 && pixels_y <= 4) {
        return 0;
    }
    // Step 0 reverse signs
    pixels_y = -pixels_y; // Reverse the sign of the number

    // Step one convert pixels to mm
    // If ^  : +y
    // If v  : -y

    const int calibration_factor_mm = 1.75; 
    int distance_mm = pixels_y * 1.75;

    // Step two convert mm to pulses
    const int pulse_per_mm = 787;
    int pulses = pulse_per_mm * distance_mm;

    return pulses;
}


// Store the incoming pixels from the camera
void VerticalSystemHandler::setPixelsY(int py) {
    pixelsY = py;
}

int VerticalSystemHandler::getPixelsY() const {
    return pixelsY;
}


void VerticalSystemHandler::setSteps(int stp) {
    steps = stp;
}

int VerticalSystemHandler::getSteps() const {
    return steps;
}


void VerticalSystemHandler::setVerticalSystemEnabled(bool enabled) {
    verticalSystemEnabled = enabled;
}

bool VerticalSystemHandler::isVerticalSystemEnabled() const {
    return verticalSystemEnabled;
}


void VerticalSystemHandler::setStepsMoved(int stp) {
    stepsMoved = stp;
}

int VerticalSystemHandler::getStepsMoved() const {
    return stepsMoved;
}