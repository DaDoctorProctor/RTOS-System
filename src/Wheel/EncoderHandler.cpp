#include "EncoderHandler.h"


EncoderHandler::EncoderHandler() {}

// Singleton instance getter
EncoderHandler& EncoderHandler::getInstance() {
    static EncoderHandler instance; // Static instance ensures it's created once
    return instance;
}

// FreeRTOS task function
void EncoderHandler::taskWrapper(void* pvParameters) {
    EncoderHandler* instance = static_cast<EncoderHandler*>(pvParameters);
    instance->taskEncoder();
}

// Task process function
void EncoderHandler::taskEncoder() {
    while(true) {
        handleEncoder();
        //vTaskDelay(1 / portTICK_PERIOD_MS); // 10ms delay
    }
}

void EncoderHandler::begin(int A, int B, int H) {
    pinA = A;
    pinB = B;
    pinH = H;

    pinMode(pinA, INPUT);
    pinMode(pinB, INPUT);
    pinMode(pinH, INPUT);

    
}

void EncoderHandler::handleEncoder() {
    EncoderFunction();
    setPosition(encoderValue);
    IRFunction();
}


void EncoderHandler::EncoderFunction(){
    int MSB = digitalRead(pinA); // MSB = most significant bit
    int LSB = digitalRead(pinB); // LSB = least significant bit

    int encoded = (MSB << 1) | LSB; // converting the 2 pin values to single number
    int sum = (lastEncoded << 2) | encoded; // adding it to the previous encoded value

    if (sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) encoderValue++;
    if (sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) encoderValue--;

    lastEncoded = encoded; // store this value for next time
}


void EncoderHandler::IRFunction(){
  int sensorValue = digitalRead(pinH);
  // Check if sensor detects something (assuming HIGH state for detection)
  if (sensorValue == HIGH && !objectDetected) {
    // Print the message only once
    Serial.println("Object detected!");
    objectDetected = true; // Set flag to true to indicate detection
  } else if (sensorValue == LOW) {
    objectDetected = false; // Reset flag when no longer detecting
  }
}


void EncoderHandler::resetPosition() {
    setPPosition(encoderValue);
    encoderValue = 0;
}


// Setter for position
void EncoderHandler::setPosition(int pos) {
    position = pos;
}

// Getter for position
int EncoderHandler::getPosition() const {
    return position;
}


// Setter for previous position
void EncoderHandler::setPPosition(int pos) {
    pposition = pos;
}

// Getter for previous position
int EncoderHandler::getPPosition() const {
    return pposition;
}


