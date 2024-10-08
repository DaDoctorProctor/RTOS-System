#include <Arduino.h>
#include "WebServerHandler/WebServerHandler.h"
#include "Wheel/WheelHandler.h"
#include "CommandHandler/CommandHandler.h"
#include "FuzzyHandler/FuzzyHandler.h"
#include "VerticalSystemHandler/VerticalSystemHandler.h"

#include "declarations.h"

void setup() {
  Serial.begin(115200);
  
  // Initialize the web server with SSID and password
  WebServerHandler& webServerHandler = WebServerHandler::getInstance();
  webServerHandler.begin(SSID,SSID_password);

  // Create a FreeRTOS task to handle client requests
  xTaskCreatePinnedToCore(
    WebServerHandler::taskWrapper,   // Task function
    "HandleClientTask",             // Task name
    4096,                           // Stack size (bytes)
    &webServerHandler,              // Task parameter
    1,                              // Priority (0 = lowest, 1 = default, 2 = highest)
    NULL,                           // Task handle
    0                               // Core number (0 or 1 for ESP32)
  );
  Serial.println("Task 1: Successful");

  CommandHandler& commandHandler = CommandHandler::getInstance();

  xTaskCreate(
    CommandHandler::taskWrapper,   // Task function
    "HandleCommandTask",             // Task name
    4096,                           // Stack size (bytes)
    &commandHandler,              // Task parameter
    1,                              // Priority (0 = lowest, 1 = default, 2 = highest)
    NULL                           // Task handle
  );
  Serial.println("Task 2: Successful");

  WheelHandler& wheelHandler = WheelHandler::getInstance();
  wheelHandler.begin(PIN_LM298_ENABLE, PIN_LM298_DIR_A, PIN_LM298_DIR_B);

  xTaskCreate(
    WheelHandler::taskWrapper,   // Task function
    "HandleWheelTask",             // Task name
    4096,                           // Stack size (bytes)
    &wheelHandler,              // Task parameter
    1,                              // Priority (0 = lowest, 1 = default, 2 = highest)
    NULL                           // Task handle
  );

  Serial.println("Task 3: Successful");

  EncoderHandler& encoderHandler = EncoderHandler::getInstance();
  encoderHandler.begin(PIN_ENCODER_A,PIN_ENCODER_B,PIN_ENCODER_H);

  xTaskCreate(
    EncoderHandler::taskWrapper,   // Task function
    "HandleWheelTask",             // Task name
    4096,                           // Stack size (bytes)
    &encoderHandler,              // Task parameter
    1,                              // Priority (0 = lowest, 1 = default, 2 = highest)
    NULL                           // Task handle
  );

  Serial.println("Task 4: Successful");

 
  FuzzyHandler& fuzzyHandler = FuzzyHandler::getInstance();
  fuzzyHandler.begin();

  xTaskCreate(
    FuzzyHandler::taskWrapper,   // Task function
    "HandleFuzzyTask",             // Task name
    4096,                           // Stack size (bytes)
    &fuzzyHandler,              // Task parameter
    1,                              // Priority (0 = lowest, 1 = default, 2 = highest)
    NULL                           // Task handle
  );

  Serial.println("Task 5: Successful");

  VerticalSystemHandler& verticalSystemHandler = VerticalSystemHandler::getInstance();
  verticalSystemHandler.begin(PIN_TB6600_DIR, PIN_TB6600_PUL, PIN_TB6600_EN, PIN_LIMIT_LOWER, PIN_LIMIT_UPPER);

  xTaskCreate(
    VerticalSystemHandler::taskWrapper,   // Task function
    "HandleVerticalTask",                // Task name
    4096,                               // Stack size (bytes)
    &verticalSystemHandler,            // Task parameter
    1,                                // Priority (0 = lowest, 1 = default, 2 = highest)
    NULL                             // Task handle
  );

  Serial.println("Task 6: Successful");

  HomeHandler& homeHandler = HomeHandler::getInstance();

  xTaskCreate(
    HomeHandler::taskWrapper,   // Task function
    "HandleHomeTask",                // Task name
    4096,                               // Stack size (bytes)
    &homeHandler,            // Task parameter
    1,                                // Priority (0 = lowest, 1 = default, 2 = highest)
    NULL                             // Task handle
  );

  Serial.println("Task 7: Successful");



}

void loop() {
  // Main loop can be used for other tasks
  vTaskDelay(1000 / portTICK_PERIOD_MS); // 1 second delay


}
