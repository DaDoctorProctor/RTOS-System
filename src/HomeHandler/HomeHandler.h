#include <Arduino.h>
#include "Wheel/EncoderHandler.h"
#include "Wheel/WheelHandler.h"

#ifndef HOME_HANDLER_H
#define HOME_HANDLER_H

class HomeHandler {
  public:

    static HomeHandler& getInstance();
    static void taskWrapper(void* pvParameters);  // Task wrapper function

    void handleHome();
    void SendHomePhaseA();
    void begin();

    void setRun(bool value);
    bool isRunning();

  private:
    void taskHome();

    HomeHandler();  // Private constructor

    int pinH;

    bool running;
};

#endif
