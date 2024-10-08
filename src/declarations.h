// declarations.h
#ifndef DECLARATIONS_H
#define DECLARATIONS_H

// const char *SSID = "TP-LINK_8542";
// const char *SSID_password = "A2217v0581X";

// const char *SSID = "unodostres";
// const char *SSID_password = "noseprogramarmevoyapymes";

const char *SSID = "iPhone";
const char *SSID_password = "6g0V-XQUu-G4Jh-Zw4n";

const int EN_W = 15;
const int IN1_W = 18;
const int IN2_W = 19;

const int ENCODER_AW = 16;
const int ENCODER_BW = 17;
const int HOME_T = 4;

// Define the devices
#define  ESP_WROOM  1
#define ESP_WROOM_S3  2

// Default device selection
#define SELECTED_DEVICE ESP_WROOM_S3

// Pin definitions for each device
#if SELECTED_DEVICE == ESP_WROOM
    #define PIN_LED             13
    #define PIN_BUTTON           2
    #define PIN_LM298_ENABLE    15
    #define PIN_LM298_DIR_A     18
    #define PIN_LM298_DIR_B     19
    #define PIN_ENCODER_A       16
    #define PIN_ENCODER_B       17
    #define PIN_ENCODER_H        4

#elif SELECTED_DEVICE == ESP_WROOM_S3
    #define PIN_LM298_ENABLE 35
    #define PIN_LM298_DIR_A 36
    #define PIN_LM298_DIR_B 37
    #define PIN_ENCODER_A 38
    #define PIN_ENCODER_B 39
    #define PIN_ENCODER_H 45

    #define PIN_TB6600_DIR 7
    #define PIN_TB6600_PUL 16
    #define PIN_TB6600_EN 15
    #define PIN_LIMIT_LOWER 18
    #define PIN_LIMIT_UPPER 10
    
#else
    #error "Please select a valid device by defining SELECTED_DEVICE as DEVICE_A, DEVICE_B, or DEVICE_C"
#endif

#endif // DECLARATIONS_H