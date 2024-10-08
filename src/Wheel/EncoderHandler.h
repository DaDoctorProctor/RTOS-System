#ifndef ENCODER_HANDLER_H
#define ENCODER_HANDLER_H

#include "Arduino.h"
#include "HomeHandler/HomeHandler.h"

class EncoderHandler {
public:
    static EncoderHandler& getInstance(); // Static method to get the singleton instance
    
    void begin(int A, int B, int H);
    void resetPosition();
    void handleHome();

    // Setters and getters for movement
    void setPosition(int pos);
    int getPosition() const;

    void setPPosition(int pos);
    int getPPosition() const;

    void handleEncoder();
    static void taskWrapper(void* pvParameters);  // Task wrapper function
    void taskEncoder();

    bool objectDetected = false; // Flag to track detection

private:
    EncoderHandler(); // Private constructor to prevent instantiation
    static EncoderHandler instance; // Static instance of Encoder
    
    int pinA;
    int pinB;
    int pinH;

    int position;
    int pposition;

    // Variables to store the encoder state
    volatile int encoderValue = 0;
    volatile int lastEncoded = 0;


    void EncoderFunction();
    
    void IRFunction();
    
    
};

#endif
